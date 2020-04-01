code_map = {
    "YEAR": "year",
    "MALE": "male population",
    "FEMALE": "female population",
    "M_MALE": "matable male population",
    "M_FEMALE": "matable female population",
    "C_PROB": "concieving probability",
    "M_AGE_START": "starting age of mating",
    "M_AGE_END": "ending age of mating",
    "MX_AGE": "maximum age",
    "MT_PROB": "mutation probability",
    "OF_FACTOR": "offspring factor",
    "AGE_DTH": "dependency of age on death",
    "FIT_DTH": "dependency of fitness on death",
    "AFR_DTH": "dependency ratio of age and fitness on death",
    "HT_SP": "dependency of height on speed",
    "HT_ST": "dependency of height on stamina",
    "HT_VT": "dependency of height on vitality",
    "WT_SP": "dependency of weight on speed",
    "WT_ST": "dependency of weight on stamina",
    "WT_VT": "dependency of weight on vitality",
    "VT_AP": "dependency of vitality on appetite",
    "VT_SP": "dependency of vitality on speed",
    "ST_AP": "dependency of stamina on appetite",
    "ST_SP": "dependency of stamina on speed",
    "TMB_AP": "theoretical maximum base appetite",
    "TMB_HT": "theoretical maximum base height",
    "TMB_SP": "theoretical maximum base speed",
    "TMB_ST": "theoretical maximum base stamina",
    "TMB_VT": "theoretical maximum base vitality",
    "TMB_WT": "theoretical maximum base appetite",
    "TM_HT": "theoretical maximum height",
    "TM_SP": "theoretical maximum speed",
    "TM_WT": "theoretical maximum weight",
    "TMM_HT": "theoretical maximum height multiplier",
    "TMM_SP": "theoretical maximum speed multiplier",
    "TMM_ST": "theoretical maximum stamina multiplier",
    "TMM_VT": "theoretical maximum vitality multiplier",
    "TMM_WT": "theoretical maximum weight multiplier",
    "SL_FACTOR": "sleep restore factor",
    "AVG_GEN": "average generation",
    "AVG_IMM": "average immunity",
    "AVG_AGE": "average age",
    "AVG_HT": "average height",
    "AVG_WT": "average weight",
    "AVGMA_AP": "average maximum appetite",
    "AVGMA_SP": "average maximum speed",
    "AVGMA_ST": "average maximum stamina",
    "AVGMA_VT": "average maximum vitality",
    "AVG_SFIT": "average static fitness",
    "AVG_DTHF": "average death factor",
    "AVG_VIS": "average vision radius",
}

def title_case(s):
    res = ''
    for word in s.split(' '):
        if word:
            res += word[0].upper()
            if len(word) > 1:
                res += word[1:]
            res += ' '
    return res.strip()

def sentence_case(s):
    res = ''
    if s:
        res += s[0].upper()
        if len(s) > 1:
            res += s[1:]
    return res.strip()