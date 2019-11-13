# DisneyStreamingServicesAssignment
Disney Streaming Services JSON Viewer Assignment

## Summary
- Developed and Built with Visual Studio 2017
- External C++ libraries used: GLFW3, Glew, Glut, JsonCPP, FreeImage
- EXE binary can be found on DropBox at: https://www.dropbox.com/sh/qp8q0w1ihere4mr/AABal2uPn6q_K1g4C09TO-yja?dl=0
- EXE zip file can be found here: https://www.dropbox.com/s/u1zsh9ds4v0nf97/TionThomas_DisneyStreamingAssignment_EXE.zip?dl=1

## Program Details:
- Program reads in an input JSON file, parses out several key pieces of game data, and outputs the data (to console and output text files) for each game in the data file. It also display a window featuring a representative image for each game that the user can scroll through (scroll the images with the <- (left arrow) and -> (right arrow)).
- Program is currently limited to a 32-bit exe as the JSONCPP library I am using is the 32-bit version.
- As a simplified way to demonstrate that the parser works for multiple files, the program allows the user to select between 1 of 3 different Json files containing the mlb stats data. Ideally, the user should be able to search by date and the program will pull the JSON data from the url dynamically. However, I was unable to get this working within the timeframe (see Notes below)
- The program collects 4 pieces of data for each game: the date of the game, the headline of the game, a blurb about the game, and the path to a thumbnail to represent the game. These are the key pieces of data necessary to display the visual list in accordance with the assignment specify
- The program outputs the results of the parsed JSON data into a file called "MLBOutput.txt" as well as the output of the JSON tree traversal (more for debugging) in a file called "JSONTree.txt"
- The statsapi.mlb JSON data contains multiple versions of the aforementioned data grouped by various criteria such as home, away, mlb etc. The current logic is to take the first instance of the data within a single game and bypass the rest. The program can be improved further to select specific data upon request
- The current submission only includes the JSON parser and a window that display a representative image for each game. The image list can be cycled using the "T" key on the keyboard. Currently only the 15 images of the default JSON game data is present. Due to time, I was unable to render the text to the window for headlines, blurbs etc. Also, the program can be improved to handle dynamic streaming of the image data from the url in accordance to a particular date the user wants to view.

## Notes:
- Attempted to use curl to read in data directly from URL and then copy to the file. Plan was for the user to specify a date as input to the program and then the program will append that data into the statsapi.mlb url to pull the data dynamically. Unfortunately, I couldn't get Curl to build on my machine in the timeframe so I removed that logic
- To render the MLB image texture, I am using GLFW, GLew, and FreeImage C++ libraries to render a quad and map the image on it as a texture.
- To collect the thumbnail paths, I am using the "photo" node as a key in which to grab the appropriate resolution cut and source. However, I noticed that some of the data does not have a photo tree for every game (sample file #3 provided only has 8 of 9). I also tried using the "image" node as a key but I saw some anomalies in the data types that made the program crash.
- Due to time, I was unable to get the text rendering working in the sample. My plan was to try to integrate the freeType library to allow free text input and position within the render target. I haven't used the library before and it was taking a bit more time than I have currently to get it integrated properly. Apologies for the omission

## Areas for Improvement
- Removing static input Json files (Read data direct from server and download single file. Reduce storage load for high number of data files)
- Removing static input image files (Read data direct from server and download single file. Reduce storage load for high number of data files)
- Add descriptive text to image window for each game
- Improve the layout of image list (image tiles, focus highlighting etc)
