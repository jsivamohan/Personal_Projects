This is a maze adventure game which mostly relied on the python pygame package as the main framework.

The game works by rendering the player as well as different mob entities the follow a specific path designated by how they turn whenever they reach the walls of the maze.

The mazes themselves are pregenerated but which maze you get on each level is randomly assigned. When you first get into a maze, the structure of the maze is invisible. It's only when you get close to walls do they make themselves appear in order to add an exploration aspect to the maze. You have a weapon you can use to fire at the mobs in order to eliminate them to score bonus points.

I also implemented a power-up system where you can collect power-ups throughout the maze and store them to be used later, they all have different abilities including illuminating the maze for a brief time to get a sense for the structure, freezing all of the mobs and making it so they don't respawn when you destroy them, and getting rid of the cooldown on your weapon. 

Your aim on each level is to get the key hidden somewhere in the maze to unlock the portal to get to the next level, each level get subsequently harder as the number of mobs will increase through each level. You have to clear 3 levels in order to win the game.
