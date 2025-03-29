#!/usr/bin/env python
# coding: utf-8

# In[ ]:


import tkinter as tk
from tkinter import ttk
import json
import os
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import time
from IPython.display import display, clear_output

# Path to your log file
folder_path = "..\\..\\..\\Saved\\Data"
files = os.listdir(folder_path)
endlessLoopActive = True
deactivateLoopAfterFirst = True # MAKE False WHEN GOING TO PRODUCTION

# Create main window
root = tk.Tk()
root.title("Real-Time Dashboard")
root.geometry("1920x1080")
# Create a frame for the graphs
frame = ttk.Frame(root)
frame.pack(fill=tk.BOTH, expand=True)
# Create a figure with 3 subplots
fig, axs = plt.subplots(2, 4, figsize=(20, 8))
# Adjust spacing
plt.subplots_adjust(left=0.05, right=0.95, top=0.95, bottom=0.05, wspace=0.3, hspace=0.4)
# Create canvas to embed Matplotlib figure in Tkinter
canvas = FigureCanvasTkAgg(fig, master=frame)
canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

def MakeGraph(graphRow, graphCol, rounds, data, label, title):
    axs[graphRow, graphCol].cla()
    axs[graphRow, graphCol].bar(rounds, data, width=1, edgecolor="blue", linewidth=0.7)
    axs[graphRow, graphCol].set_xlabel("Round Number")
    axs[graphRow, graphCol].set_ylabel(label)
    axs[graphRow, graphCol].set_title(title)
    axs[graphRow, graphCol].grid(True)

def visualizeRounds(rounds):
    roundNumbers = []
    savingLastLogTimes = []
    roundGameTimes = []
    roundRealTimes = []
    roundTicks = []
    gameSpeeds = []
    roundNumbersNoSoftLock = []
    savingLastLogTimesNoSoftLock = []
    roundGameTimesNoSoftLock = []
    roundRealTimesNoSoftLock = []
    roundTicksNoSoftLock = []
    gameSpeedsNoSoftLock = []
    for round_data in rounds:
        roundNumbers.append(round_data['RoundNumber'])
        savingLastLogTimes.append(round_data['SavingLastLogTime'])            
        roundGameTime = 0
        roundRealTime = 0
        roundTick = 0
        for log in round_data['Logs']:
            roundGameTime += log['GameTime']
            roundRealTime += log['RealTime']
            roundTick += log['TickCount']
        roundGameTimes.append(roundGameTime)
        roundRealTimes.append(roundRealTime)
        roundTicks.append(roundGameTime)
        gameSpeeds.append(roundGameTime/roundRealTime)
        if round_data['GameEnding'] != "EGameEnding::SoftLocked":
            roundNumbersNoSoftLock.append(round_data['RoundNumber'])
            savingLastLogTimesNoSoftLock.append(round_data['SavingLastLogTime'])
            roundGameTimeNoSoftLock = 0
            roundRealTimeNoSoftLock = 0
            roundTickNoSoftLock = 0
            for log in round_data['Logs']:
                roundGameTimeNoSoftLock += log['GameTime']
                roundRealTimeNoSoftLock += log['RealTime']
                roundTickNoSoftLock += log['TickCount']
            roundGameTimesNoSoftLock.append(roundGameTimeNoSoftLock)
            roundRealTimesNoSoftLock.append(roundRealTimeNoSoftLock)
            roundTicksNoSoftLock.append(roundTickNoSoftLock)
            gameSpeedsNoSoftLock.append(roundGameTimeNoSoftLock/roundRealTimeNoSoftLock)
     # Clear previous plot
    # clear_output(wait=True)
    # No SoftLock
    MakeGraph(0, 0, roundNumbersNoSoftLock, savingLastLogTimesNoSoftLock, "LogTime", "LogTimesNoSoftLock")
    MakeGraph(0, 1, roundNumbersNoSoftLock, roundGameTimesNoSoftLock, "GameTime", "GameTimesNoSoftLock")
    MakeGraph(0, 2, roundNumbersNoSoftLock, roundRealTimesNoSoftLock, "RealTime", "RealTimesNoSoftLock")
    MakeGraph(0, 3, roundNumbersNoSoftLock, gameSpeedsNoSoftLock, "GameSpeed", "GameSpeedsNoSoftLock")
    # With Softlock
    MakeGraph(1, 0, roundNumbers, savingLastLogTimes, "LogTime", "LogTimes")
    MakeGraph(1, 1, roundNumbers, roundGameTimes, "GameTime", "GameTimes")
    MakeGraph(1, 2, roundNumbers, roundRealTimes, "RealTime", "RealTimes")
    MakeGraph(1, 3, roundNumbers, gameSpeeds, "GameSpeed", "GameSpeeds")

try:
    while endlessLoopActive:
        endlessLoopActive = not deactivateLoopAfterFirst
        if files:
            firstfile = os.path.join(folder_path, files[0])  # Ensure full path
            print(f"Opening file: {firstfile}")
            with open(firstfile, 'r') as file:
                rounds = []
                for line in file:
                    # Try to load each line as a separate JSON object
                    try:
                        json_obj = json.loads(line.strip())  # Strip any extra whitespace or newlines
                        rounds.append(json_obj)
                    except json.JSONDecodeError as e:
                        print(f"Error parsing line: {line}. Error: {e}")

                visualizeRounds(rounds)
                 # Refresh the canvas
                canvas.draw()
                time.sleep(5)  # Wait for 5 seconds before updating
        else:
            print("No files found in the directory!")
except KeyboardInterrupt:
    print("Graph update stopped by user.")

root.mainloop()


# In[ ]:




