import numpy as np

def Accuracy(label, pred):
    ########################################################################################
    # TODO : Complete the code to calculate the accuracy for prediction.
    #         [Input]
    #         - label : (N, ), Correct label with 0 (negative) or 1 (positive)
    #         - hypo  : (N, ), Predicted score between 0 and 1
    #         [output]
    #         - Acc : (scalar, float), Computed accuracy score
    # ========================= EDIT HERE =========================
    Acc = None

    correct = 0
    for i in range(0, len(label)):
        if label[i] == pred[i]:
            correct += 1
    Acc = correct / len(label)


    # =============================================================
    return Acc

def Precision(label, pred):
    ########################################################################################
    # TODO : Complete the code to calculate the Precision for prediction.
    #         you should consider that label = 1 is positive. 0 is negative
    #         Notice that, if you encounter the divide zero, return 1
    #         [Input]
    #         - label : (N, ), Correct label with 0 (negative) or 1 (positive)
    #         - hypo  : (N, ), Predicted score between 0 and 1
    #         [output]
    #         - precision : (scalar, float), Computed precision score
    # ========================= EDIT HERE =========================
    precision = None

    tp = 0
    fp = 0
    for i in range(0, len(label)):
        if pred[i] == 1:
            if label[i] == pred[i]:
                tp += 1
            else:
                fp += 1
    try:
        precision = tp / (tp+fp)
    except ZeroDivisionError:
        precision = 1


    # =============================================================
    return precision

def Recall(label, pred):
    ########################################################################################
    # TODO : Complete the code to calculate the Recall for prediction.
    #         you should consider that label = 1 is positive. 0 is negative
    #         Notice that, if you encounter the divide zero, return 1
    #         [Input]
    #         - label : (N, ), Correct label with 0 (negative) or 1 (positive)
    #         - hypo  : (N, ), Predicted score between 0 and 1
    #         [output]
    #         - recall : (scalar, float), Computed recall score
    # ========================= EDIT HERE =========================
    recall = None
    tp = 0
    fn = 0
    for i in range(len(label)):
        if label[i] == 1:
            if pred[i] == label[i]:
                tp += 1
            else:
                fn += 1
    
    try:
        recall = tp / (tp+fn)
    except ZeroDivisionError:
        recall = 1


    # =============================================================
    return recall

def F_measure(label, pred):
    ########################################################################################
    # TODO : Complete the code to calculate the F-measure score for prediction.
    #         you can erase the code. (F_score = 0.)
    #         Notice that, if you encounter the divide zero, return 1
    #         [Input]
    #         - label : (N, ), Correct label with 0 (negative) or 1 (positive)
    #         - hypo  : (N, ), Predicted score between 0 and 1
    #         [output]
    #         - F_score : (scalar, float), Computed F-score score
    # ========================= EDIT HERE =========================
    F_score = None
    r = Recall(label, pred)
    p = Precision(label, pred)
    alpha = 0.5
    F_score = 1/((1/p) * alpha+ (1/r)*(1-alpha))
    # =============================================================
    return F_score

def MAP(label, hypo, at = 10):
    ########################################################################################
    # TODO : Complete the code to calculate the MAP for prediction.
    #         Notice that, hypo is the real value array in (0, 1)
    #         MAP (at = 10) means MAP @10
    #         [Input]
    #         - label : (N, K), Correct label with 0 (incorrect) or 1 (correct)
    #         - hypo  : (N, K), Predicted score between 0 and 1
    #         - at: (int), # of element to consider from the first. (TOP-@)
    #         [output]
    #         - Map : (scalar, float), Computed MAP score
    # ========================= EDIT HERE =========================
    Map = 0

    for line_label, line_hypo in zip(label, hypo):
        ap = 0
        count = 0
        new_array = np.array([line_label, line_hypo])
        new_sort = new_array[:,new_array[1,:].argsort()]
        new_sort = np.array([new_sort[0], new_sort[1]])
        new_sort = np.flip(new_sort, 1)
        
        correct = 0
        for ans in line_label:
            if ans == 1:
                correct += 1

        for i in range(at):
            if new_sort[0][i] == 1:
                count += 1
                p = count/(i+1)
                ap += p
        
        ap /= correct
        Map += ap

    Map /= len(label)
    # =============================================================
    return Map

def nDCG(label, hypo, at = 10):
    ########################################################################################
    # TODO : Complete the each code to calculate the nDCG for prediction.
    #         you can erase the code. (dcg, idcg, ndcg = 0.)
    #         Notice that, hypo is the real value array in (0, 1)
    #         nDCG (at = 10 ) means nDCG @10
    #         [Input]
    #         - label : (N, K), Correct label with 0 (incorrect) or 1 (correct)
    #         - hypo  : (N, K), Predicted score between 0 and 1
    #         - at: (int), # of element to consider from the first. (TOP-@)
    #         [output]
    #         - Map : (scalar, float), Computed nDCG score


    def DCG(label, hypo, at=10):
        # ========================= EDIT HERE =========================
        dcg = 0
        new_array = np.array([label, hypo])
        new_sort = new_array[:,new_array[1,:].argsort()]
        new_sort = np.array([new_sort[0], new_sort[1]])
        new_sort = np.flip(new_sort, 1)
        for i in range(2, at+2):
            if new_sort[0][i-2] == 1:
                dcg += (1/np.log2(i))

        # =============================================================
        return dcg

    def IDCG(label, hypo, at=10):
        # ========================= EDIT HERE =========================
        idcg = 0
        correct = 0

        for ans in label:
            if ans == 1:
                correct += 1
        
        for i in range(2, min(correct+2, at+2)):
            idcg += 1/np.log2(i)
        # =============================================================
        return idcg
    # ========================= EDIT HERE =========================
    ndcg = 0

    for line_label, line_hypo in zip(label, hypo):
        idcg = IDCG(line_label, line_hypo, at)
        dcg = DCG(line_label, line_hypo, at)
        ndcg += (dcg / idcg)
    
    ndcg /= len(label)
    # =============================================================
    return ndcg

# =============================================================== #
# ===================== DO NOT EDIT BELOW ======================= #
# =============================================================== #

def evaluation_test1(label, pred, at = 10):
    result = {}

    result['Accuracy '] = Accuracy(label, pred)
    result['Precision'] = Precision(label, pred)
    result['Recall   '] = Recall(label, pred)
    result['F_measure'] = F_measure(label, pred)

    return result

def evaluation_test2(label, hypo, at = 10):
    result = {}

    result['MAP  @%d'%at] = MAP(label, hypo, at)
    result['nDCG @%d'%at] = nDCG(label, hypo, at)

    return result
