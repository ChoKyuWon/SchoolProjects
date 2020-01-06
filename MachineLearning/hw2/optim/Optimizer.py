import numpy as np

"""
DO NOT EDIT ANY PARTS OTHER THAN "EDIT HERE" !!! 

[Description]
__init__ - Initialize necessary variables for optimizer class
input   : gamma, epsilon
return  : X

update   - Update weight for one minibatch
input   : w - current weight, grad - gradient for w, lr - learning rate
return  : updated weight 
"""

class SGD:
    def __init__(self, gamma, epsilon):
        # ========================= EDIT HERE =========================
        pass


        # =============================================================

    def update(self, w, grad, lr):
        updated_weight = None
        # ========================= EDIT HERE =========================
        for j in range(0,len(w)):
            w[j] = w[j] - lr * grad[j]
        return w


        # =============================================================
        return updated_weight

class Momentum:
    def __init__(self, gamma, epsilon):
        # ========================= EDIT HERE =========================
        self.gamma = gamma
        self.vt = None
        pass


        # =============================================================

    def update(self, w, grad, lr):
        updated_weight = None
        # ========================= EDIT HERE =========================
        if self.vt is None:
            self.vt = [1] * len(w)
        
        for j in range(0, len(w)):
            self.vt[j] = self.gamma * self.vt[j] - lr * grad[j]
        w = w + self.vt
        return w



        # =============================================================
        return updated_weight


class RMSProp:
    # ========================= EDIT HERE =========================
    def __init__(self, gamma, epsilon):
        # ========================= EDIT HERE =========================
        self.gamma = gamma
        self.epsilon = epsilon
        self.G = None
        pass


        # =============================================================

    def update(self, w, grad, lr):
        updated_weight = None
        # ========================= EDIT HERE =========================
        if self.G is None:
            self.G = [1] * len(w)

        for j in range(0, len(w)):
            self.G[j] = self.gamma * self.G[j] + (1-self.gamma)*(grad[j] **2)
            w[j] = w[j] - lr / ((self.G[j] + self.epsilon) ** 0.5) * grad[j]
        
        return w       



        # =============================================================
        return updated_weight
