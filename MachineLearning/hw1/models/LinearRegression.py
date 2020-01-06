import numpy as np

class LinearRegression:
    def __init__(self, num_features):
        self.num_features = num_features
        self.W = np.zeros((self.num_features, 1))

    def train(self, x, y, epochs, batch_size, lr, optim):
        final_loss = None   # loss of final epoch

        # Training should be done for 'epochs' times with minibatch size of 'batch_size'
        # The function 'train' should return the loss of final epoch
        # Loss of an epoch is calculated as an average of minibatch losses

        # ========================= EDIT HERE ========================
        # xline 과 n번째 y가 매칭됨. f(xline)=yi
        final_loss=0
        num_data=len(y)
        k=0
       

        def dlossF(k, j):
            s=0
            size = batch_size
            for Xi, Yi in zip(x[k:k+batch_size], y[k:k+batch_size]):
                fx=np.transpose(Xi).dot(self.W)
                s = s + (fx-Yi)*Xi[j]
            if (num_data - k) < batch_size:
                size = num_data - k
            return s/size
        
        for iterative in range(0, epochs):
            k = k + batch_size
            if k == num_data:
                k = batch_size
            grad = np.zeros((self.num_features, 1))
            for j in range(0, self.num_features):
                grad[j] = dlossF(k, j)
            self.W =  optim.update(self.W, grad, lr)


        # ============================================================
        return final_loss

    def eval(self, x):
        pred = None

        # Evaluation Function
        # Given the input 'x', the function should return prediction for 'x'

        # ========================= EDIT HERE ========================
        ylist=[]
        for xline in x:
            y = np.transpose(xline).dot(self.W)
            ylist.append(y[0])
        pred = np.array(ylist)

        # ============================================================
        return pred
