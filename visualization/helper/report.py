from matplotlib.backends.backend_pdf import PdfPages
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
from typing import NoReturn
from datetime import datetime
from .enums import Kingdom as KingdomE
from .language import get_kingdom, title_case
from .report_animal import get_figs as get_animal_figs
from .report_plant import get_figs as get_plant_figs
from matplotlib.gridspec import GridSpec
from pathlib import Path
import matplotlib.image as mimage
from matplotlib.offsetbox import OffsetImage, AnnotationBbox

def create_pdf(figs: list, savepath: str) -> NoReturn:
    with PdfPages(savepath) as pdf:
        for fig in figs:
            plt.figure(fig.number)
            pdf.savefig()
    print("Report generated at", savepath)

def get_firstpage(kind_count: int, year_count: int) -> Figure:
    root_path = Path(__file__).absolute().parent.parent.parent

    fig = plt.figure()
    fig.set_size_inches(8, 10)
    gs = GridSpec(1, 1, figure=fig)

    logo_path = root_path / 'docs' / 'assets' / 'img' / 'eco_black.png'
    logo = mimage.imread(str(logo_path))
    imagebox = OffsetImage(logo, zoom = 0.06)

    ax = fig.add_subplot(gs[0, 0])
    ax.add_artist(AnnotationBbox(imagebox, (0.221, 0.76), frameon=False))
    ax.text(0.2, 0.7, 'Simulation Report', dict(size=30))
    ax.text(0.2, 0.66, 'for {} species over {} years'.format(kind_count, year_count), dict(size=10))
    timestamp = datetime.now().strftime("%d/%m/%Y")
    ax.text(0.675, 0.025, "Generated on {}".format(timestamp), dict(size=10))
    ax.get_yaxis().set_visible(False)
    ax.get_xaxis().set_visible(False)

    return fig


def get_kind_firstpage(kingdom: str, kind: str) -> Figure:
    fig = plt.figure()
    fig.set_size_inches(8, 10)
    gs = GridSpec(1, 1, figure=fig)

    ax = fig.add_subplot(gs[0, 0])
    ax.text(0.2, 0.6, '{}'.format(title_case(kind)), dict(size=30))
    ax.text(0.2, 0.56, 'of {} kingdom'.format(get_kingdom(kingdom)), dict(size=10))
    ax.get_yaxis().set_visible(False)
    ax.get_xaxis().set_visible(False)

    return fig


def generate_figs(data: dict) -> list:
    figs = []
    kind_count = 0
    year_count = 0

    for kingdom, kind_map in data.items():
        kind_count += len(kind_map)
        for kind, year_map in kind_map.items():
            kind_page = get_kind_firstpage(kingdom, kind)
            figs.append(kind_page)

            year_count = max(year_count, len(year_map))
            data_pages = []
            if kingdom == KingdomE.ANIMAL:
                data_pages = get_animal_figs(year_map)
                
            # if kingdom == KingdomE.PLANT:
            #     data_pages = get_plant_figs(year_map)
            figs.extend(data_pages)

    front_page = get_firstpage(kind_count, year_count)
    figs.insert(0, front_page)
    
    return figs

def generate_and_save_report(data: dict, savepath: str) -> NoReturn:
    figs = generate_figs(data)
    create_pdf(figs, savepath)