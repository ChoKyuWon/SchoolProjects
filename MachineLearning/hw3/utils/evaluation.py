import numpy as np

def Accuracy(label, pred):
    Acc = 0.
    ########################################################################################
    # TODO : Complete the code to calculate the accuracy for prediction.
    #         you can erase the code. (Acc = 0.)
    # ========================= EDIT HERE =========================

    # =============================================================
    return Acc

def Precision(label, pred):
    precision = 0.
    ########################################################################################
    # TODO : Complete the code to calculate the Precision for prediction.
    #         you should consider that label = 1 is positive. 0 is negative
    #         you can erase the code. (precision = 0.)
    #         Notice that, if you encounter the divide zero, return 1
    # ========================= EDIT HERE =========================

    # =============================================================
    return precision

def Recall(label, pred):
    recall = 0.
    ########################################################################################
    # TODO : Complete the code to calculate the Recall for prediction.
    #         you should consider that label = 1 is positive. 0 is negative
    #         you can erase the code. (recall = 0.)
    #         Notice that, if you encounter the divide zero, return 1
    # ========================= EDIT HERE =========================

    # =============================================================
    return recall

def F_measure(label, pred):
    F_score = 0.
    ########################################################################################
    # TODO : Complete the code to calculate the F-measure score for prediction.
    #         you can erase the code. (F_score = 0.)
    #         Notice that, if you encounter the divide zero, return 1
    # ========================= EDIT HERE =========================

    # =============================================================
    return F_score

def MAP(label, hypo, at = 10):
    Map = 0.
    ########################################################################################
    # TODO : Complete the code to calculate the MAP for prediction.
    #         you can erase the code. (Map = 0.)
    #         Notice that, hypo is the real value array in (0, 1)
    #         MAP (at = 10) means MAP @10
    # ========================= EDIT HERE =========================

    # =============================================================
    return Map

def nDCG(label, hypo, at = 10):
    ########################################################################################
    # TODO : Complete the each code to calculate the nDCG for prediction.
    #         you can erase the code. (dcg, idcg, ndcg = 0.)
    #         Notice that, hypo is the real value array in (0, 1)
    #         nDCG (at = 10 ) means nDCG @10
    def DCG(label, hypo, at=10):
        dcg = 0.
        # ========================= EDIT HERE =========================

        # =============================================================
        return dcg

    def IDCG(label, hypo, at=10):
        idcg = 1.0
        # ========================= EDIT HERE =========================

        # =============================================================
        return idcg
    ndcg = 0.
    # ========================= EDIT HERE =========================

    # =============================================================
    return ndcg

def evaluation_test1(label, pred, at = 10):
    # pred has values[0,  1]

    result = {}

    result['Accuracy '] = Accuracy(label, pred)
    result['Precision'] = Precision(label, pred)
    result['Recall   '] = Recall(label, pred)
    result['F_measure'] = F_measure(label, pred)

    return result

def evaluation_test2(label, hypo, at = 10):

    # hypo has values[0 ~ 1]
    result = {}

    result['MAP @%d'%at] = MAP(label, hypo, at)
    result['nDCG @%d'%at] = nDCG(label, hypo, at)

    return result
