import numpy as np
import matplotlib.pyplot as p
import matplotlib.ticker as MT
import matplotlib.cm as CM

def test_add(a,b):
    return a+b

def listLoop(*lst):
    flag = 0
    print('the string is below: ')
    for i in lst:
         if flag == 0:
             x = i[0]
             y = i[1]
             arr = np.zeros((x, y))
             flag = 1
             k = 0
             print('xxxxx: ',x,' yyyyyyyyy : ',y)
         else:
             arr[k, 0:y] = i
             k = k + 1
    # arr1 = arr.T
    # print(type(arr1))
    # print(type(arr))
    ax = scaledimage(arr.T)
    p.show()

def scaledimage(W, pixwidth=1, ax=None, grayscale=True):
    '''
    :param W:  intensity matrix to visualize
    :param pixwidth: size of each W element
    :param ax:matplotlib AXES TO DRAW ON
    :param grayscale:   use grayscale color map
    :return:
    '''
    # N,M = row,col
    (N, M) = W.shape
    if ax == None:
        ax = p.figure().gca()
    exts = (0, pixwidth * M, 0, pixwidth * N)
    if grayscale:
        ax.imshow(W, interpolation='nearest', cmap=CM.gray, extent=exts)
    else:
        ax.imshow(W, interpolation='nearest', extent=exts)
    ax.xaxis.set_major_locator(MT.NullLocator())
    ax.yaxis.set_major_locator(MT.NullLocator())
    return ax