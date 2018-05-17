# Huidong Hwang and Chris Knoll
# 5/8/16
# Classifies the given attr

import numpy as np

from sklearn.ensemble import RandomForestClassifier
from sklearn.cross_validation import StratifiedKFold,cross_val_score

from fancyimpute import SoftImpute


def num(x):
    '''Returns a number with its correct type'''
    try:
        return int(x)
    except ValueError:
        return float(x)


def rangeparse():
    '''Returns an array that contains max-min and min for each attribute'''
    rangearray = []
    with open("attr.txt","r+") as f:
        for line in f:
            line = line.replace(" (?)","")
            line = line.replace(",?","")
            
            if line[0] == "C":
                index1 = line.find("cont ")
                index2 = line.find("..")
                dslice = [num(line[index1+5:index2]),num(line[index2+2:-2])]
            elif line[0] == "D":
                index = line.find(":")
                dslice = [num(line[index+2]),num(line[-3:-2])]
            else:
                break
            n,x = dslice
            rangearray.append([x-n,n])
    return rangearray


def parse(filename,classfile=""):
    '''Returns a 2-tuple of the training set and the class values'''
    ra=rangeparse()
    attr = [] 
    target = []
    with open(filename,"r+") as f:
        for line in f:
            dslice = [np.nan if x == "?" else num(x) for x in line.split()]
            attr.append(dslice[:-1])
            if classfile == "":
                target.append(dslice[-1:])
    if classfile != "":
        with open(classfile,"r+") as f:
            target = [num(x) for x in f]
    attr = [[(x - ra[i][1])/ra[i][0] for i,x in enumerate(line) if x != np.nan] for line in attr]
    attr = np.array(attr)
    target = np.array(target).ravel()
    attr = SoftImpute(verbose=False).complete(attr)
    return (attr,target)


def train(X,y):
    '''Returns 2-tuple of a feature selected and trained RandomForestClassifier and an array of relevant features'''
    rf = RandomForestClassifier(n_estimators=1300, min_samples_leaf=7,oob_score=True,criterion='gini')
    skf = StratifiedKFold(y,n_folds=5,shuffle=True)

    rf = rf.fit(X,y)
    features = [i for i,x in enumerate(rf.feature_importances_) if x > 0.0022]
    X = X[:,features]

    rf = rf.fit(X,y)
    print("Training accuracy (with KFolds and feature select): {}".format(cross_val_score(rf,X,y, cv=skf)))
    return (rf,features)


def main():
    attr,target = parse("train.mv.txt")
    prelimset,prelimclass = parse("prelim-mv-noclass.txt","prelim-class.txt")
    testset,__ = parse("final-mv-noclass.txt")

    attr = np.concatenate((attr,prelimset))
    target = np.concatenate((target,prelimclass))

    rf,features = train(attr,target)
    testset = testset[:,features]
    prediction = rf.predict(testset)
    with open("prediction.txt","w+") as f:
        for x in prediction:
            f.write("{}\n".format(x))
    return


if __name__ == "__main__":
    main()
