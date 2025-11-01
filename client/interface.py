import subprocess

filePath = ""
countFiles1 = 0
bgColour = "#1b1f33"
bgColourLight = "#3d456e"
colour2 = "#7ccbe6"
colour2light = "#bdeeff"


def parcourir():
    global filePath
    global countFiles1 
    filePath = filedialog.askdirectory()

    if filePath != "":
        countFiles1 += 1
        listeFichiersCompiles.insert(countFiles1, filePath + "/.pio/build/megaatmega2560/firmware.hex")
    
    print("parcourir")
    

def envoyer():
    #à faire
    print("envoyer")

def listboxSelect(event):
    buttonEnvoyer.config(state = "normal")
    buttonEnvoyer.config(bg = colour2)

def generer():
    cle = "CLÉ" #génère la clé
    textClePrivee.insert("1", cle)
    buttonEnvoyer2.config(state = "normal")

from tkinter import *
from tkinter import filedialog

# window
window = Tk() #instantiate an instance of a window
screen_width = window.winfo_screenwidth()
screen_height = window.winfo_screenheight()

window.geometry(f"{screen_width}x{screen_height}") 
window.title("Shacks 2025") #à changer???
window.config(bg = bgColour)

# PARTIE 1 : FICHIER

# labels
labelText = Label(window, text = "Veuillez choisir le dossier du projet", font = ('Calibri', 11), bg = bgColour, fg = colour2)
labelText.place(x = 15, y = 13)

labelTextGros = Label(window, text = "Projet compilé :", font = ('Calibri', 20, 'bold'), bg = bgColour, fg = colour2)
labelTextGros.place(x = 15, y = 35)

labelBorder = Label(window, bg = colour2, padx = 80, pady = 15)
labelBorder.place(x = 22, y =257)

# buttons + listbox
buttonParcourir = Button(window, 
    text = "Choisir le dossier", 
    command=parcourir, 
    font = ('Calibri', 15, 'bold'),
    bg = bgColour,
    fg = colour2,
    #activeforeground = colour2light,
    #borderwidth = 0,
    #highlightbackground = colour2
    )
buttonParcourir.place(x = 25, y = 260)

listeFichiersCompiles = Listbox(window, font = ('Calibri', 10), width = 130)
listeFichiersCompiles.place(x = 15, y = 80)
listeFichiersCompiles.bind('<<ListboxSelect>>', listboxSelect)

buttonEnvoyer = Button(window, 
    text = "Envoyer au Arduino", 
    command=envoyer, 
    font = ('Calibri', 15, 'bold'), 
    state = "disabled",
    bg = colour2light,
    fg = bgColour,
    activebackground = colour2light,
    disabledforeground = bgColourLight,
    highlightthickness = 3,
    highlightbackground = bgColour
    )
buttonEnvoyer.place(x = 195, y = 260)


# PARTIE 2 : CLÉ

# labels

labelBorder = Label(window, bg = colour2, padx = 75, pady = 15)
labelBorder.place(x = 22, y =590)

labelTextGros2 = Label(window, text = "Clés (privée et publique) :", font = ('Calibri', 20, 'bold'), bg = bgColour, fg = colour2)
labelTextGros2.place(x = 15, y = 350)

# text

textClePrivee = Text(window, 
    height = 10, 
    width = 130, 
    bg = 'WHITE',
    font = ('Calibri', 11),
    state = 'disabled'
    )
textClePrivee.place(x = 15, y = 397)

# buttons

buttonGenerer = Button(window, 
    text = "Générer les clés", 
    command=generer, 
    font = ('Calibri', 15, 'bold'),
    bg = bgColour,
    fg = colour2,
    #activeforeground = colour2light,
    #borderwidth = 0,
    #highlightbackground = colour2
    )
buttonGenerer.place(x = 25, y = 593)

buttonEnvoyer2 = Button(window, 
    text = "Envoyer au Arduino", 
    command=envoyer, 
    font = ('Calibri', 15, 'bold'), 
    state = "disabled",
    bg = colour2light,
    fg = bgColour,
    activebackground = colour2light,
    disabledforeground = bgColourLight,
    highlightthickness = 3,
    highlightbackground = bgColour
    )
buttonEnvoyer2.place(x = 187, y = 593)


window.mainloop() #place window on computer screen and listens for events
