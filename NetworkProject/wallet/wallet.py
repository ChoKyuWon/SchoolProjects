from cryptos import *
import hashlib as hs
import ecdsa as ecc
import requests
import shutil
import sys

class Cipher():

    def sxor(self, s1, s2):
        result=b""
        for k, ch in zip(s1, s2):
            result = result + bytes([k ^ ch])
        return result

    def encrypt(self, password, msg):
        l = len(msg)
        key = (sha256(password)*100).encode()[0:l]
        Ct = self.sxor(key,msg)
        return Ct
    
    def decrypt(self, password, ciphertext):
        l = len(ciphertext)
        key = (sha256(password)*100).encode()[0:l]
        msg = self.sxor(key,ciphertext)
        return msg
    
class wallet:
    def __init__(self, address=None, mode="init", testnetVal=False):
        self.crypto = Cipher()
        self.c = Bitcoin(testnet=False)
        self.testnetVal = testnetVal
        if testnetVal is True:
            self.c = Bitcoin(testnet=True)

        if mode is "init":
            self.genprivkey()
        elif mode is "getkey":
            self.getkeyfromFile(address)

    def genprivkey(self):
        self.priv_ecc = ecc.SigningKey.generate()
        self.priv = sha256(self.priv_ecc.to_string().hex())
        self.pub = self.c.privtopub(self.priv)
        self.addr = self.c.pubtoaddr(self.pub)
        self.exportprivtoFile()
        return(self.addr)

    def exportprivtoFile(self):
        password=input("input your password: ")
        ct=self.crypto.encrypt(password, self.priv_ecc.to_pem())
        with open('./'+self.addr+'.pem', 'wb') as f:
            f.write(ct)
    
    def getkeyfromFile(self, filename):
        print("input your wallet's address on "+filename)
        try:
            f = open('./'+filename+'.pem', 'rb')
        except FileNotFoundError:
            print("Private File Not Found. Generate New Private Key? (Y/N):")
            while True:
                yn=input()
                if yn is 'y' or yn is 'Y' or yn is 'yes' or yn is 'YES':
                    self.genprivkey()
                    return
                elif yn is 'n' or yn is'N' or yn is 'No' or yn is 'no' or yn is  'NO':
                    return
                else:
                    print("Please Enter Y/N.")
        ct=f.read()
        while True:
            password=input("input password: ")
            pk=self.crypto.decrypt(password, ct)
            try:
                self.priv_ecc = ecc.SigningKey.from_pem(pk)
                break
            except ValueError:
                print("password is worg! please retry.")
        self.priv = sha256(self.priv_ecc.to_string().hex())
        self.pub = self.c.privtopub(self.priv)
        self.addr = self.c.pubtoaddr(self.pub)
    
    def qrcode_create(self, filename):
        if self.testnetVal is False:
            url='https://blockchain.info/qr?data='+self.addr
        else:
            url='http://chart.googleapis.com/chart?cht=qr&chl=bitcoin%'+self.addr+'&choe=UTF-8&chs=300x300'
        
        r=requests.get(url)
        if r.status_code == 200:
            with open('./'+filename+'.jpg', 'wb') as f:
                for chunk in r.iter_content(1024):
                    f.write(chunk)
        else:
            print("error code:"+str(r.status_code))
            print("     "+url)

    def sendBT(self, sendTo, Amount, memo):
        self.c.send(self.priv, sendTo, Amount)
        return
    
    def history(self):
        return self.c.history(self.addr)

class user:
    def __init__(self):
        self.mywallet = None
        self.iswallet= False
        self.testnet = False

    def help(self):
        print("Usage:")
        print(" help : print some helpful menual")
        print(" genwallet : Generate new wallet. If you want make testnet wallet, type 'genwallet testnet'")
        print(" getwallet [address] : Get wallet from address. You need [address].pem file. If you have no pem file, generate new wallet.")
        print(" send : send BTC to someone")
        print(" addr : print your current wallet's address")
        print(" history : Show your wallet's breif history.")
        print(" transaction : Show your wallet's all transaction.")
        print(" getbalance : print current balance of wallet")
        print(" testnet : testnet on. If enter testnet again, testnet off.")
        print(" qrcode : make qrcode of your current wallet's address.")
        print(" exit : exit program.")

    def userfunction(self):
        command = input('>')
        if 'help' in command:
            self.help()
        elif 'testnet' in command:
            if self.testnet == True:
                self.testnet  = False
                print('Testnet if Off!')
            else:
                self.testnet  = True
                print('Testnet if On!')

        elif 'genwallet' in command:
            if self.testnet is True:
                print("testnet wallet will be created!")
                self.mywallet=wallet(testnetVal=True)
                self.iswallet = True
                print(self.mywallet.addr)
            else:
                print("mainnet wallet will be created!")
                self.mywallet=wallet()
                self.iswallet = True
                print(self.mywallet.addr)

        elif 'getwallet' in command:
            try:
                if self.testnet is True:
                    print("testnet wallet will be imported!")
                    command_li = command.split(' ')
                    self.mywallet = wallet(command_li[1], 'getkey', True)
                    self.iswallet = True
                    print(self.mywallet.addr)
                else:
                    print("mainnet wallet will be imported!")
                    command_li = command.split(' ')
                    self.mywallet = wallet(command_li[1], 'getkey')
                    self.iswallet = True
                    print(self.mywallet.addr)
            except IndexError:
                print("Usage : getwallet [address]")

        elif 'send' in command:
            if self.iswallet is False:
                print('Please genwallet first!')
            else:
                sendTo = input("What is reciver's Address? : ")
                sendTo = sendTo.strip()
                balance = self.mywallet.history()['final_balance']
                Amount = input("Amount of BTC : ")
                if Amount == 'all':
                    Amount = balance - 10000
                    memo = input('memo : ')
                    self.mywallet.sendBT(sendTo, Amount, memo)
                    
                else:
                    try:
                        Amount = int(float(Amount)*100000000)
                        if Amount >= int(balance) :
                            print("You need more balance!")
                            return
                    except:
                        print("send value is invalid!")
                        return
                    if Amount == 0:
                        print("Send only larger then 10^(-8)!")
                        return
                    memo = input('memo : ')
                    self.mywallet.sendBT(sendTo, Amount, memo)


        elif 'addr' in command:
            if self.iswallet is False:
                print('Please genwallet first!')
            else:
                print(self.mywallet.addr)

        elif 'history' in command:
            if self.iswallet is False:
                print('Please genwallet first!')
            else:
                h = self.mywallet.history()
                resstr=''
                for tx in h["txs"]:
                    resstr = resstr + tx['inputs'][0]['prev_out']['addr']
                    resstr = resstr + ' to '
                    resstr = resstr + tx['out'][0]['addr']
                    resstr = resstr + ', '
                    resstr = resstr + str(int(tx['out'][0]['value'])/100000000)
                    resstr = resstr + 'BTC\n'
                print(resstr.rstrip())

        elif 'transaction' in command:
            if self.iswallet is False:
                print('Please genwallet first!')
            else:
                h = self.mywallet.history()
                for tx in h["txs"]:
                    print(tx)
        
        elif 'getbalance' in command:
            if self.iswallet is False:
                print('Please genwallet first!')
            else:
                h = self.mywallet.history()
                print(h['final_balance']/100000000, 'BTC')

        elif 'exit' in command:
            return -1

        elif 'qrcode' in command:
            if self.iswallet is False:
                print('Please genwallet first!')
            else:
                self.mywallet.qrcode_create(self.mywallet.addr)
        
        else:
            print("Wrong command!")
            self.help()


def wallet_init():
    myuser = user()
    while True:
        if myuser.userfunction() == -1:
            break


if __name__ == '__main__':
    wallet_init()