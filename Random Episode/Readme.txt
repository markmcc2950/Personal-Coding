RANDOM EPISODE PICKER:

The goal of this program is to use .txt documents to scan for episodes picked randomly by the program, and then check a master list to see if the episode has been viewed.

If an episode has been viewed, the program continues searching until it finds a new episode. If it searches [n] number of times, where n is the total amount of episodes between all files, then all episodes have been viewed and the program terminates. 

To create a list of episodes, the user has to manually enter the list of all seasons and episodes, in the format coded in the program. 

For example:
	HIMYM.txt
		Season: x, Episode: y, "Episode Title"
	
	- When the program parses this file, it randomly picks an (x, y) and fills in these values. It then searches the list of HIMYM.txt to see if there's a match with that picked x, y value. 
	- Once it finds a match, it looks in the master file ("Viewed.txt") to see if there's a match there too. In the event of a match, the program generates a new (x, y) to scan again.
	- This continues until the program finds a match in the files of a series (How I Met Your Mother, Parks & Recreation, etc...) that does not yet exist in the master file.
	- The program then writes the picked episode with the matching (x, y) values to the master file so it won't be selected again as a duplicate in the future.

The eventual goal is to set up a local storage system for direct network streaming. Currently I could investigate Hulu or Netflix scripting to find the episodes, but local network would be easier and more reliable for any changes to the websites.



Program made and original idea solely by Mark McCullough