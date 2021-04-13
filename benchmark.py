import matplotlib.pyplot as plt
import subprocess

d = {}

pages = 100
progs = ["sort", "scan", "focus"]
algs = ["rand", "fifo", "custom"]
frames = [1, 2, 10, 25, 50, 100]
linestyles = {"page_faults": "solid", "disk_reads": "dotted", "disk_writes": "dashed"}
colors = {"rand":"blue", "fifo":"orange", "custom":"green"}

for prog in progs:
  d[prog] = {}
  for alg in algs:
    d[prog][alg] = {}
    possible_frames = frames.copy()
    if prog == "sort":
      possible_frames.remove(1)
    for frame in possible_frames:
      dd = {}
      d[prog][alg][frame] = dd
      ret_val = subprocess.run(['./virtmem', str(pages), str(frame), alg, prog], stdout=subprocess.PIPE).stdout.decode('utf-8')
      dd["page_faults"] = ret_val.split(" ")[6]
      dd["disk_reads"] = ret_val.split(" ")[9]
      dd["disk_writes"] = ret_val.split(" ")[12].strip()

for prog in progs:
  fig = plt.figure()
  ax = fig.add_subplot(111)
  #plt.subplots_adjust(left=2, top=2, right=2, bottom=2)
  for alg in algs:
    dtest = d[prog][alg]
    dummy_lines = []
    for line in linestyles.keys():
        # ADD THIS LINE TO SEE BETTER CUSTOM SORT, ALSO COMMENT OUT LINES FURTHER DOWN
        # if (prog != 'sort' or alg != 'custom' or (line != 'page_faults' and line != 'disk_reads')):
      ax.plot([int(k) for k in list(dtest.keys())], [int(dtest[key][line]) for key in dtest.keys()], color=colors[alg], linestyle=linestyles[line])
      dummy_lines.append(ax.plot([], [], color="black", linestyle=linestyles[line])[0])
  """plt.plot([d[prog][alg] for alg in algs])
  plt.savefig(prog+".png")
  plt.xlabel("Number of frames")
  plt.clear()
"""

  lines = ax.get_lines()

  #l1 = ax.legend(lines[0:3], linestyles)

  ##### COMMENT THESE LINES OUT TO SEE BETTER CUSTOM SORT
  l1 = ax.legend(dummy_lines, linestyles, loc=1)
  l2 = ax.legend([lines[i] for i in [0,6,12]], colors, loc=3)
  ##### __________________________________________________

  ax.add_artist(l1)
  #fig.tight_layout()
  ax.set_title(f"Num frames vs num pagefault/diskread/diskwrite for {prog} program across algs", fontsize=10)
  ax.set_xlabel("Num Frames")
  ax.set_ylabel("Num events (check legend)")
  #plt.show()
  plt.savefig(f"test{prog}.png")
  ax.clear()
