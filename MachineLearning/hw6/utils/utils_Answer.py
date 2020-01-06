import numpy as np

def Gini_index(Y_data):
    gini = 0
    #=========    Edit here    ==========
    #
    myset = list(set(Y_data))
    plist = [0]*len(myset)
    
    for y in Y_data:
        for s in myset:
            if y == s:
                plist[myset.index(s)] += 1

    for i in range(len(plist)):
        plist[i] /= len(Y_data)
        if plist[i] == 0:
            continue
        gini += plist[i]*(1-plist[i])

    #
    #
    #====================================
    return gini

def Entropy(Y_data):

    entropy = 0
    # =====    Edit here    ========
    #
    myset = list(set(Y_data))
    plist = [0]*len(myset)
    
    for y in Y_data:
        for s in myset:
            if y == s:
                plist[myset.index(s)] += 1
    for i in range(len(plist)):
        plist[i] /= len(Y_data)
        if plist[i] == 0:
            continue
        entropy -= plist[i]*np.log2(plist[i])

    #
    #
    # ==============================
    return entropy

def impurity_func(Y_data, criterion):

    if criterion == 'gini':
        return Gini_index(Y_data)

    elif criterion == 'entropy':
        return Entropy(Y_data)

def Finding_split_point(df, feature, criterion):

    col_data = df[feature]
    Y = df.values[:, -1]
    distinct_data = np.unique(col_data)

    split_point = distinct_data[0]
    min_purity = 1

    for idx, val in enumerate(distinct_data):
        less_idx = (col_data < val)

        y0 = Y[less_idx]
        y1 = Y[~less_idx]

        p0 = len(y0) / len(Y)
        p1 = len(y1) / len(Y)

        purity = np.sum([p0 * impurity_func(y0, criterion), p1 * impurity_func(y1, criterion)])

        if min_purity > purity:
            min_purity = purity
            split_point = val

    return split_point

def Gaussian_prob(x, mean, std):
    '''
    :param x: input value: X
    :param mean: the mean of X
    :param std: the standard deviation of X
    :return: probaility (X) ~ N(μ, σ^2)
    '''
    ret = 0
    # ========      Edit here         ==========
    pi = np.pi
    e = np.e
    ret = 1/((2*pi*std)**(1/2)) * (np.e)**((-1)*(x-mean)**2/(2*std))
    # =========================================
    return ret