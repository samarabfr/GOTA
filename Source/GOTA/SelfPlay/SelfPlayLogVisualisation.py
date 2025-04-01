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
import numpy as np

# Path to your log file
folder_path = "..\\..\\..\\Saved\\Data"
files = os.listdir(folder_path)
endlessLoopActive = True
deactivateLoopAfterFirst = False # MAKE False WHEN GOING TO PRODUCTION

# Create main window
root = tk.Tk()
root.title("Real-Time Dashboard")
root.geometry("1920x1080")
# Create a frame for the graphs
frame = ttk.Frame(root)
frame.pack(fill=tk.BOTH, expand=True)
# Create a figure with 3 subplots
fig, axs = plt.subplots(4, 8, figsize=(30, 10))
# Adjust spacing
plt.subplots_adjust(left=0.05, right=0.95, top=0.95, bottom=0.05, wspace=0.3, hspace=1)
# Create canvas to embed Matplotlib figure in Tkinter
canvas = FigureCanvasTkAgg(fig, master=frame)
canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

def MakeBarGraph(graphRow, graphCol, rounds, data, label, title):
    axs[graphRow, graphCol].cla()
    axs[graphRow, graphCol].bar(rounds, data, width=1, edgecolor="blue", linewidth=1)
    axs[graphRow, graphCol].set_xlabel("Round Number")
    axs[graphRow, graphCol].set_ylabel(label)
    axs[graphRow, graphCol].set_title(title)
    axs[graphRow, graphCol].grid(True)

def MakeLineGraph(graphRow, graphCol, rounds, data, label, title):
    axs[graphRow, graphCol].cla()
    axs[graphRow, graphCol].plot(rounds, data)
    axs[graphRow, graphCol].set_xlabel("Round Number")
    axs[graphRow, graphCol].set_ylabel(label)
    axs[graphRow, graphCol].set_title(title)
    axs[graphRow, graphCol].grid(True)

def MakeHistogramm(graphRow, graphCol, data, label, title):
    axs[graphRow, graphCol].cla()
    axs[graphRow, graphCol].hist(data, bins=50, color='blue', edgecolor='black')
    axs[graphRow, graphCol].set_xlabel(label)
    axs[graphRow, graphCol].set_ylabel("Count")
    axs[graphRow, graphCol].set_title(title)
    axs[graphRow, graphCol].grid(True)

def visualizeRounds(rounds):
    # with softlock
    roundNumbers = []
    savingLastLogTimes = []
    roundGameTimes = []
    roundRealTimes = []
    roundTicks = []
    gameSpeeds = []
    # settlement
    colonistsCountBuildings = []
    colonistsCountCivilianBuildings = []
    colonistsCountArmyBuildings = []
    colonistsCountDefenseBuildings = []
    colonistsCountCivilians = []
    colonistsCountWoodcutter = []
    colonistsCountForager = []
    colonistsCountBuilder = []
    colonistsArmies = []
    colonistsPop = []
    colonistsFood = []
    colonistsWood = []
    colonistsStone = []
    colonistsFoodIncome = []
    colonistsWoodIncome = []
    colonistsStoneIncome = []
    nativesCountBuildings = []
    nativesCountCivilianBuildings = []
    nativesCountArmyBuildings = []
    nativesCountDefenseBuildings = []
    nativesCountCivilians = []
    nativesCountWoodcutter = []
    nativesCountForager = []
    nativesCountBuilder = []
    nativesArmies = []
    nativesPop = []
    nativesFood = []
    nativesWood = []
    nativesStone = []
    nativesFoodIncome = []
    nativesWoodIncome = []
    nativesStoneIncome = []
    # averages
    averageSpan = 100
    currentColonistsWins = 0
    colonistsWinrates = []
    currentNativesWins = 0
    nativesWinrates = []
    currentSoftlocks = 0
    softlockRates = []

    # gathering data
    for round_data in rounds:
        # with softlock
        roundNumbers.append(round_data['RoundNumber'])
        savingLastLogTimes.append(round_data['SavingLastLogTime'])
        lastLog = round_data['Logs'][-1]
        colonistsCountBuildings.append(lastLog['Colony']['CountBuildings'])
        colonistsCountCivilianBuildings.append(lastLog['Colony']['CountCivilianBuildings'])
        colonistsCountArmyBuildings.append(lastLog['Colony']['CountArmyBuildings'])
        colonistsCountDefenseBuildings.append(lastLog['Colony']['CountDefenseBuildings'])
        colonistsCountCivilians.append(lastLog['Colony']['CountCivilians'])
        colonistsCountWoodcutter.append(lastLog['Colony']['CountWoodcutter'])
        colonistsCountForager.append(lastLog['Colony']['CountForager'])
        colonistsCountBuilder.append(lastLog['Colony']['CountBuilder'])
        colonistsArmies.append(lastLog['Colony']['Armies'])
        colonistsPop.append(lastLog['Colony']['Pop'])
        colonistsFood.append(lastLog['Colony']['Food'])
        colonistsWood.append(lastLog['Colony']['Wood'])
        colonistsStone.append(lastLog['Colony']['Stone'])
        colonistsFoodIncome.append(lastLog['Colony']['FoodIncome'])
        colonistsWoodIncome.append(lastLog['Colony']['WoodIncome'])
        colonistsStoneIncome.append(lastLog['Colony']['StoneIncome'])
        nativesCountBuildings.append(lastLog['Tribe']['CountBuildings'])
        nativesCountCivilianBuildings.append(lastLog['Tribe']['CountCivilianBuildings'])
        nativesCountArmyBuildings.append(lastLog['Tribe']['CountArmyBuildings'])
        nativesCountDefenseBuildings.append(lastLog['Tribe']['CountDefenseBuildings'])
        nativesCountCivilians.append(lastLog['Tribe']['CountCivilians'])
        nativesCountWoodcutter.append(lastLog['Tribe']['CountWoodcutter'])
        nativesCountForager.append(lastLog['Tribe']['CountForager'])
        nativesCountBuilder.append(lastLog['Tribe']['CountBuilder'])
        nativesArmies.append(lastLog['Tribe']['Armies'])
        nativesPop.append(lastLog['Tribe']['Pop'])
        nativesFood.append(lastLog['Tribe']['Food'])
        nativesWood.append(lastLog['Tribe']['Wood'])
        nativesStone.append(lastLog['Tribe']['Stone'])
        nativesFoodIncome.append(lastLog['Tribe']['FoodIncome'])
        nativesWoodIncome.append(lastLog['Tribe']['WoodIncome'])
        nativesStoneIncome.append(lastLog['Tribe']['StoneIncome'])
        if round_data['GameEnding'] == "EGameEnding::ColonistsWon":
            currentColonistsWins += 1
        if round_data['GameEnding'] == "EGameEnding::NativesWon":
            currentNativesWins += 1
        if round_data['GameEnding'] == "EGameEnding::SoftLocked":
            currentSoftlocks += 1
        if (round_data['RoundNumber'] + 1) % averageSpan == 0:
            colonistsWinrates.append(currentColonistsWins / averageSpan)
            nativesWinrates.append(currentNativesWins / averageSpan)
            softlockRates.append(currentSoftlocks / averageSpan)
            currentColonistsWins = 0
            currentNativesWins = 0
            currentSoftlocks = 0
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


    num_groups = len(roundNumbers) // averageSpan
    averageGameTimes = np.mean(np.array(roundGameTimes[:num_groups * averageSpan]).reshape(-1, averageSpan), axis=1)
    averageRealTimes = np.mean(np.array(roundRealTimes[:num_groups * averageSpan]).reshape(-1, averageSpan), axis=1)
    averageGameSpeeds = np.mean(np.array(gameSpeeds[:num_groups * averageSpan]).reshape(-1, averageSpan), axis=1)
    averageRoundNumbers = np.arange(averageSpan, len(averageGameTimes) * averageSpan + 1, averageSpan)

    # Clear previous plot
    # clear_output(wait=True)

    MakeLineGraph(0, 0, averageRoundNumbers, averageGameTimes, "GameTime", "GameTimes")
    MakeLineGraph(0, 1, averageRoundNumbers, averageRealTimes, "RealTime", "RealTimes")
    MakeLineGraph(0, 2, averageRoundNumbers, averageGameSpeeds, "GameSpeed", "GameSpeeds")
    MakeLineGraph(0, 3, averageRoundNumbers, softlockRates, "SoftlockRate", "SoftlockRates")
    MakeLineGraph(0, 4, averageRoundNumbers, colonistsWinrates, "Winrate", "ColonistsWinrates")
    MakeLineGraph(0, 5, averageRoundNumbers, nativesWinrates, "Winrate", "NativesWinrate")

    MakeHistogramm(1, 0, roundGameTimes, "GameTime", "GameTimes")
    MakeHistogramm(1, 1, roundRealTimes, "RealTime", "RealTimes")
    MakeHistogramm(1, 2, gameSpeeds, "GameSpeed", "GameSpeeds")
    MakeHistogramm(1, 3, savingLastLogTimes, "LogTime", "LogTimes")

    MakeBarGraph(2, 0, roundNumbers, colonistsCountBuildings, "BuildingsCount", "ColonistsBuildings")
    MakeHistogramm(2, 1, colonistsCountBuildings, "BuildingsCount", "ColonistsBuildings")
    MakeBarGraph(2, 2, roundNumbers, colonistsCountCivilians, "CivilianCount", "ColonistsCivilians")
    MakeHistogramm(2, 3, colonistsCountCivilians, "CivilianCount", "ColonistsCivilians")
    MakeBarGraph(2, 4, roundNumbers, colonistsArmies, "ArmyCount", "ColonistsArmies")
    MakeHistogramm(2, 5, colonistsArmies, "ArmyCount", "ColonistsArmies")
    MakeBarGraph(2, 6, roundNumbers, colonistsPop, "Pop", "ColonistsPop")
    MakeHistogramm(2, 7, colonistsPop, "Pop", "ColonistsPop")

    MakeBarGraph(3, 0, roundNumbers, nativesCountBuildings, "BuildingsCount", "NativesBuildings")
    MakeHistogramm(3, 1, nativesCountBuildings, "BuildingsCount", "NativesBuildings")
    MakeBarGraph(3, 2, roundNumbers, nativesCountCivilians, "CivilianCount", "NativesCivilians")
    MakeHistogramm(3, 3, nativesCountCivilians, "CivilianCount", "NativesCivilians")
    MakeBarGraph(3, 4, roundNumbers, nativesArmies, "ArmyCount", "NativesArmies")
    MakeHistogramm(3, 5, nativesArmies, "ArmyCount", "NativesArmies")
    MakeBarGraph(3, 6, roundNumbers, nativesPop, "Pop", "NativesPop")
    MakeHistogramm(3, 7, nativesPop, "Pop", "NativesPop")

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
        else:
            print("No files found in the directory!")
        time.sleep(5)  # Wait for 5 seconds before updating
except KeyboardInterrupt:
    print("Graph update stopped by user.")

root.mainloop()


# In[ ]:




