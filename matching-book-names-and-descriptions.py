
import math
emptywords="if the of be in on is are then".split(" ")
n=int(raw_input())
Titles=[]
for i in range(n):
    title=" "+raw_input().lower()+" "
    
    #for ew in emptywords:
        #title=title.replace(" "+ew+" "," ")
    Titles.append(title)
    #title=title.strip()
    #Titles.append(list(set(title.split(" "))))
    


raw_input()

Description=[]
Words=[]
WordsTF=[]
for i in range(n):
    Description.append(raw_input().lower())
    wd=Description[i].split(" ")
    Words.append(list(set(wd)) )
    f=[0]*len(Words[i])
    for w in range(len(Words[i])):
        count=Description[i].count(Words[i][w])
        f[w]=count*1.0/len(wd)
    WordsTF.append(f)
Countincorpus=0
TFIDF=[]
for i in range(n):
    description={}
    for w in range(len(Words[i])):
        Countincorpus=0
        for s in Description:
            if(s.find(Words[i][w])!=-1):
                Countincorpus+=1
        #print Words[i][w]," = ",n*1.0/Countincorpus

        description[Words[i][w]]=WordsTF[i][w]*math.log(n*1.0/Countincorpus)
    TFIDF.append(description)


for i in range(len(Titles)):
    
    scores=[0]*n
    maxscore=0
    indxmax=0
    for w in TFIDF[i]:
        for j in range(n):
            if(Titles[j].find(w)!=-1):
                scores[j]+=TFIDF[i][w]
                if(scores[j]>maxscore):
                    maxscore=scores[j]
                    indxmax=j               

    
                
    print indxmax+1
        

