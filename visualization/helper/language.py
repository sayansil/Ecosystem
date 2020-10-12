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
