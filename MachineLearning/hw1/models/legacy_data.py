
        '''
        num_data=len(y)
        def dlossF(j):
            sum=0
            for i in range(0, batch_size):
                fx=np.transpose(x[i])
                fx=fx.dot(self.W)
                sum=sum+(fx-y[i])*x[i][j]
            return (sum/batch_size)
        
        for num in range(0, epochs):
            for i in range(0, self.num_features):
                delta = dlossF(i)
                self.W[i] = self.W[i] - lr*delta

        for i in range(0, num_data):
            fx=np.transpose(x[i]).dot(self.W)
            final_loss = final_loss + (fx-y[i])*(fx-y[i])
        num_data=len(y)
        def dlossF(j, k):
            sum=0
            for Xi, Yi in zip(x[k:k+batch_size], y[k:k+batch_size]):
                fx=np.transpose(Xi)
                fx=fx.dot(self.W)
                sum=sum+(fx-Yi)*Xi[j]
            if num_data-k<batch_size:
                size = num_data-k
            else:
                size = batch_size
            return (sum/size)

        for k in range(0,num_data,batch_size):
            for iterative in range(0,epochs):
                for j in range(0, self.num_features):
                    delta=dlossF(j, k)
                    self.W[j]=self.W[j] -lr*delta
                optim.update(self.W, grad, lr)

        for i in range(0, num_data):
            fx=np.transpose(x[i]).dot(self.W)
            final_loss = final_loss + (fx-y[i])*(fx-y[i])
        '''