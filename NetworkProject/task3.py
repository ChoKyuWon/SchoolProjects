import hashlib as hs
import ecdsa as ecc
import base58

with open('address.txt','r') as f:
    count=0
    for line in f:
    #if True:
        line_decode=base58.b58decode(line)
        checksum=line_decode[21:]
        line_decode=line_decode[:-4]
        #print(line_decode.hex()+"  "+checksum.hex())
        sha1=hs.sha256(line_decode).digest()
        sha2=hs.sha256(sha1).digest()
        #print(sha2.hex())
        
        if sha2[0:4] != checksum:
            count=count+1
            print(line)

        
    print(count)

    class tools:
    def privtopub(private_key):
        public_key = private_key.get_verifying_key()
        return public_key

    def pubtoaddr(public_key):
        x=public_key.to_string()[:32]
        y=public_key.to_string()[32:]
        comkey=0

        if y[-1]|254 is 255:
            comkey='03'+x.hex()

        elif y[-1]|254 is 254:
            comkey='02'+x.hex()
        
        #task2

        #compkey to sha 356, ripemd160
        h=hs.sha256()
        h.update(bytes.fromhex(comkey))
        puhash=hs.new('ripemd160', h.digest()).digest()
        #push double sha256
        puhash=b'\00'+puhash
        sha1=hs.sha256(puhash).digest()
        sha2=hs.sha256(sha1).digest()
        #put checksum
        puhash=puhash+sha2[0:4]

        #base58 encoding
        addr=base58.b58encode(puhash)
        return addr
