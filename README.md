# PokeAI

Install instructions can be found at https://github.com/Adam-Petersen/PokeAI-vba-m. This repo is a submodule of PokeAI-vba-m and contains most of the code for PokeAI.

This is a project I've worked on for the past several months on and off. The goal of the project is to create an AI that can complete the Game Boy game Pokemon Red completely unassisted. Early stages of the project are focused on learning the inner workings of Pokemon Red, studying its ROM and RAM maps, and creating utilities for executing game actions.

The AI can be considered an AI in the loosest interpretation of AI, due to the fact it doesn't currently utilize any machine learning, but instead utilizes a pre-determined routine to perform actions. This is something that I will work on at a later stage in the project.

The AI is divided into two parts, which I call Game AI and Battle AI. Game AI is responsible for navigating the world, talking to NPCs, and interacting with items. Battle AI is responsible for winning battles, catching pokemon, and evenly leveling the player's pokemon. Currently, the Battle AI is mostly implemented, and the Game AI is in early stages.

Finally, this project has also sparked my curiosity in how Pokemon Red stores its tile data and how it displays the graphics you see on screen based on this data. I've written a lot of code that extrapolates map graphic data from ROM and creates BMP images of individual tiles, maps, and the entire game world. If you're interested an example, check out https://github.com/Adam-Petersen/PokeAI-vba-m/blob/master/img/world/world_mashed_named.bmp. This image is a collection of every map stored in ROM, with the outside world maps stitched together and corresponding map labels added. All of this was done dynamically.

There is still much work to be done with this project, but I'm happy with the results thus far, and have loved working with the vba-m emulator and Pokemon Red, one of my favorite games of all time.
