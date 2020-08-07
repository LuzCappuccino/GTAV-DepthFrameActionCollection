import numpy as np
import os
import sys
from matplotlib import pyplot as plt
#%matplotlib inline


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: {0} <raw_fn>".format(sys.argv[0]))
        sys.exit()

    fn = sys.argv[1]

    with open(fn, "rb") as f:
        buffer = f.read()
    im = np.frombuffer(buffer, dtype="float32").reshape(int(720), -1)
    plt.imshow(im)
    plt.axis("off");

    plt.show()
