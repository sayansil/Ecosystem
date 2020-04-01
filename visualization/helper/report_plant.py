import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
from matplotlib.backends.backend_pdf import PdfPages
from datetime import datetime
import os


def generate_pdf(df, kind, savepath='../outputs/'):
    figs = []
    fig = get_start_page(kind)
    figs.append(fig)
    fig = get_mortality_graphs(df)
    figs.append(fig)
    fig = get_demographic_graphs(df)
    figs.append(fig)
    fig = get_copulation_graphs(df)
    figs.append(fig)
    fig = get_dependency_graphs(df)
    figs.append(fig)
    fig1, fig2 = get_average_graphs(df)
    figs.append(fig1)
    figs.append(fig2)
    fig1, fig2 = get_theoretical_graphs(df)
    figs.append(fig1)
    figs.append(fig2)

    timestamp = datetime.now().strftime("%d-%m-%Y %H-%M-%S%p")
    savepath += 'report_' + kind + ' ' + timestamp + '.pdf'
    with PdfPages(savepath) as pdf:
        for fig in figs:
            plt.figure(fig.number)
            pdf.savefig()
    print("Report generated at", savepath)

def get_start_page(kind):
    fig = plt.figure()
    fig.set_size_inches(8, 10)
    gs = GridSpec(1, 1, figure=fig)

    ax = fig.add_subplot(gs[0, 0])
    ax.text(0.2, 0.5, 'Simulation Report', dict(size=30))
    ax.text(0.025, 0.025, "Species: " + kind, dict(size=10))
    ax.get_yaxis().set_visible(False)
    ax.get_xaxis().set_visible(False)

    return fig

def get_mortality_graphs(df):
    """
        AGE_DTH
        FIT_DTH
        AFR_DTH
        MX_AGE
    """
    fig = plt.figure()
    fig.set_size_inches(8, 10)
    fig.subplots_adjust(hspace=0.3)
    fig.subplots_adjust(wspace=0.3)
    fig.suptitle('Mortality Graphs')
    gs = GridSpec(3, 2, figure=fig)

    ax = fig.add_subplot(gs[0, 0])
    ax.set_title('Age affecting Death')
    x = df.index
    y = df['AGE_DTH']
    ax.plot(x, y, '-r')
    ax.set_ylabel("Factor")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[0, 1])
    ax.set_title('Fitness affecting Death')
    x = df.index
    y = df['FIT_DTH']
    ax.plot(x, y, '-b')
    ax.set_ylabel("Factor")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[1, :])
    ax.set_title('Age vs. Fitness affecting Death')
    x = df.index
    y = df['AFR_DTH']
    ax.plot(x, y, '-b')
    ax.set_ylabel("Ratio")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[2, :])
    ax.set_title('Max age with time')
    x = df.index
    y = df['MX_AGE']
    ax.plot(x, y, '-b')
    ax.set_ylabel("Max age")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return fig

def get_demographic_graphs(df):
    """
        POP
    """
    fig = plt.figure()
    fig.set_size_inches(8, 10)
    fig.subplots_adjust(hspace=0.3)
    fig.subplots_adjust(wspace=0.3)
    fig.suptitle('Demographic Graphs')
    gs = GridSpec(1, 1, figure=fig)

    ax = fig.add_subplot(gs[:, :])
    ax.set_title('Population')
    x = df.index
    y = df['POP']
    ax.plot(x, y, '-b')
    ax.set_ylabel("Population")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return fig

def get_copulation_graphs(df):
    """
        M_POP
        M_AGE_START
        M_AGE_END
        C_PROB
        MT_PROB
        OF_FACTOR
    """
    fig = plt.figure()
    fig.set_size_inches(8, 10)
    fig.subplots_adjust(hspace=0.3)
    fig.subplots_adjust(wspace=0.3)
    fig.suptitle('Copulation Graphs')
    gs = GridSpec(4, 2, figure=fig)

    ax = fig.add_subplot(gs[0, :])
    ax.set_title('Matable population')
    x = df.index
    y = df['M_POP']
    ax.plot(x, y, '-r')
    ax.set_ylabel("Matable Population")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[1, 0])
    ax.set_title('Mating Start')
    x = df.index
    y = df['M_AGE_START']
    ax.plot(x, y, '-b')
    ax.set_ylabel("Age")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[1, -1])
    ax.set_title('Mating End')
    x = df.index
    y = df['M_AGE_END']
    ax.plot(x, y, '-r')
    ax.set_ylabel("Age")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[2, 0])
    ax.set_title('Mutation')
    x = df.index
    y = df['MT_PROB']
    ax.plot(x, y, '-r')
    ax.set_ylabel("Probability")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[2, -1])
    ax.set_title('Conceive')
    x = df.index
    y = df['C_PROB']
    ax.plot(x, y, '-b')
    ax.set_ylabel("Probability")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[3, :])
    ax.set_title('Multiple offprings')
    x = df.index
    y = df['OF_FACTOR']
    ax.plot(x, y, '-b')
    ax.set_ylabel("Factor")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return fig

def get_dependency_graphs(df):
    """
        HT_VT
        WT_VT
    """
    fig = plt.figure()
    fig.set_size_inches(8, 10)
    fig.subplots_adjust(hspace=0.3)
    fig.subplots_adjust(wspace=0.3)
    fig.suptitle('Dependency Graphs')
    gs = GridSpec(1, 1, figure=fig)

    ax = fig.add_subplot(gs[0, 0])
    ax.set_title('Factors affecting Vitality')
    x = df.index
    y = df['HT_VT']
    ax.plot(x, y, '-r', label='Height')
    x = df.index
    y = df['WT_VT']
    ax.plot(x, y, '-b', label='Weight')
    ax.set_ylabel("Factor")
    ax.legend(loc="upper right")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return fig

def get_average_graphs(df):
    """
        AVG_GEN
        AVG_AGE
        AVG_HT
        AVG_WT
        AVG_SFIT
        AVG_IMM
        AVG_DTHF
        AVGMA_VT
    """
    fig1 = plt.figure()
    fig1.set_size_inches(8, 10)
    fig1.subplots_adjust(hspace=0.3)
    fig1.subplots_adjust(wspace=0.3)
    fig1.suptitle('Average Graphs 1')
    gs = GridSpec(2, 2, figure=fig1)

    ax = fig1.add_subplot(gs[0, 0])
    ax.set_title('Generation')
    x = df.index
    y = df['AVG_GEN']
    ax.plot(x, y, '-g')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[0, 1])
    ax.set_title('Age')
    x = df.index
    y = df['AVG_AGE']
    ax.plot(x, y, '-m')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[1, 0])
    ax.set_title('Height')
    x = df.index
    y = df['AVG_HT']
    ax.plot(x, y, '-b')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[1, 1])
    ax.set_title('Weight')
    x = df.index
    y = df['AVG_WT']
    ax.plot(x, y, '-r')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    fig2 = plt.figure()
    fig2.set_size_inches(8, 10)
    fig2.subplots_adjust(hspace=0.3)
    fig2.subplots_adjust(wspace=0.3)
    fig2.suptitle('Average Graphs 2')
    gs = GridSpec(2, 3, figure=fig2)

    ax = fig2.add_subplot(gs[0, 0])
    ax.set_title('Static Fitness')
    x = df.index
    y = df['AVG_SFIT']
    ax.plot(x, y, '-b')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig2.add_subplot(gs[0, 1])
    ax.set_title('Immunity')
    x = df.index
    y = df['AVG_IMM']
    ax.plot(x, y, '-r')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig2.add_subplot(gs[0, 2])
    ax.set_title('Death Factor')
    x = df.index
    y = df['AVG_DTHF']
    ax.plot(x, y, '-m')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig2.add_subplot(gs[1, 0:3])
    ax.set_title('Max vitality at age')
    x = df.index
    y = df['AVGMA_VT']
    ax.plot(x, y, '-c')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return fig1, fig2

def get_theoretical_graphs(df):
    """
        TMB_HT
        TMB_WT
        TM_HT
        TM_WT
        TMB_VT
        TMM_VT
        TMM_HT
        TMM_WT
    """
    fig1 = plt.figure()
    fig1.set_size_inches(8, 10)
    fig1.subplots_adjust(hspace=0.3)
    fig1.subplots_adjust(wspace=0.3)
    fig1.suptitle('Theoretical Graphs 1')
    gs = GridSpec(3, 2, figure=fig1)

    ax = fig1.add_subplot(gs[0, 0])
    ax.set_title('Max base height')
    x = df.index
    y = df['TMB_HT']
    ax.plot(x, y, '-b')
    ax.set_ylabel("meters")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[0, -1])
    ax.set_title('Max height')
    x = df.index
    y = df['TM_HT']
    ax.plot(x, y, '-b')
    ax.set_ylabel("meters")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[1, 0])
    ax.set_title('Max base weight')
    x = df.index
    y = df['TMB_WT']
    ax.plot(x, y, '-r')
    ax.set_ylabel("kg")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[1, -1])
    ax.set_title('Max weight')
    x = df.index
    y = df['TM_WT']
    ax.plot(x, y, '-r')
    ax.set_ylabel("kg")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[2, 0:2])
    ax.set_title('Max base vitality')
    x = df.index
    y = df['TMB_VT']
    ax.plot(x, y, '-g')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    fig2 = plt.figure()
    fig2.set_size_inches(8, 10)
    fig2.subplots_adjust(hspace=0.3)
    fig2.subplots_adjust(wspace=0.3)
    fig2.suptitle('Theoretical Graphs 2')
    gs = GridSpec(2, 2, figure=fig2)

    ax = fig2.add_subplot(gs[0, 0:2])
    ax.set_title('Max vitality multiplier')
    x = df.index
    y = df['TMM_VT']
    ax.plot(x, y, '-m')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig2.add_subplot(gs[1, 0])
    ax.set_title('Max height multiplier')
    x = df.index
    y = df['TMM_HT']
    ax.plot(x, y, '-r')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig2.add_subplot(gs[1, -1])
    ax.set_title('Max weight multiplier')
    x = df.index
    y = df['TMM_WT']
    ax.plot(x, y, '-b')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return fig1, fig2
