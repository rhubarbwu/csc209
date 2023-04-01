from os import listdir

import matplotlib.pyplot as plt
import matplotlib.scale as scale
import numpy as np
import pandas as pd

data = dict()

for file in listdir("."):
    if not file.endswith(".log"): continue
    name = file.replace(".log", "")
    data[name] = {"real": [], "sys": [], "user": []}

    with open(file, "r") as f:
        lines = f.readlines()
    
    for line in lines:
        parts = line.split()
        if len(parts) == 0: continue
        tag = parts[0]
        match tag:
            case "real" | "user" | "sys":
                value = parts[1].replace("0m", "").replace("s","")
                data[name][tag].append(float(value))
            case other: continue


fig, axs = plt.subplots(1, 4, figsize=(15, 4))
for i, name in enumerate(data.keys()):
    df = pd.DataFrame(data[name])
    axs[i].set_title(name)
    df.plot(xlabel="number of threads (logarithmic)", ylabel="time (s)", ax=axs[i])

plt.savefig("plots.png", bbox_inches = 'tight', pad_inches = 0.1)
