import hashlib as hs
import ecdsa as ecc
import base58

h=hs.sha256()
#hs.new('ripemd160', data).digest()
#h.update(string.encode())
#bytes.fromhex(data)

private_key = ecc.SigningKey.generate()
public_key = private_key.get_verifying_key()

x=public_key.to_string()[:32]
y=public_key.to_string()[32:]
comkey=0

if y[-1]|254 is 255:
    #print("y is odd")
    comkey='03'+x.hex()
    #print("compressed key is 0x"+x.hex())

elif y[-1]|254 is 254:
    #print("y is even")
    comkey='02'+x.hex()
    #print("compressed key is 0x" + comkey)

#task2
print(comkey.encode())

#compkey to sha 356, ripemd160
h.update(bytes.fromhex(comkey))
puhash=hs.new('ripemd160', h.digest()).digest()
print(puhash)

#push double sha256
puhash=b'\00'+puhash
sha1=hs.sha256(puhash).digest()
sha2=hs.sha256(sha1).digest()
print(sha2)

#put checksum
puhash=puhash+sha2[0:4]
print(puhash)

#base58 encoding
addr=base58.b58encode(puhash)
print(addr)