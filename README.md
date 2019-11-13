# DisneyStreamingServicesAssignment
Disney Streaming Services JSON Viewer Assignment

## Program Details:
- Program reads in an input JSON file, parses out several key pieces of game data, and outputs the data for each game in the data file
- Program is currently limited to a 32-bit exe as the JSONCPP library I am using is the 32-bit version.
- As a simplified way to demonstrate that the parser works for multiple files, the program allows the user to select between 1 of 3 different Json files containing the mlb stats data. Ideally, the user should be able to search by date and the program will pull the JSON data from the url dynamically. However, I was uanble to get this working within the timeframe (see Notes below)
- The program collects 4 pieces of data for each game: the date of the game, the headline of the game, a blurb about the game, and the path to a thumbnail to represent the game. These are the key pieces of data necessary to display the visual list in accordance with the assignment specify
- The statsapi.mlb JSON data contains multiple versions of the aforementioned data grouped by various criteria such as home, away, mlb etc. The current logic is to take the first instance of the data within a single game and bypass the rest. The program can be improved further to select specific data upon request
- The current submission only includes the JSON parser as I was not able to get the visual list rendered properly using GLFW and OpenGL within the timeframe.

## Notes:
- Attempted to use curl to read in data directly from URL and then copy to the file. Plan was for the user to specify a date as input to the program and then the program will append that data into the statsapi.mlb url to pull the data dynamically. Unfortunately, I couldn't get Curl to build on my machine in the timeframe so I removed that logic
- Admittedly, it has been a while since I tried to program OpenGL and I apologize for the omission of the visual rendering of the list. I attempted to integrate GLFW to create a single window with a quad. That part worked and the next step was to display a texture of the MLB image on the quad (and resize to 150%). However, issues with getting GLFW and Glad to build as well as errors with my shader to display the texture on the quad prevented me from including that feature.
- To collect the thumbnail paths, I am using the "photo" node as a key in which to grab the appropriate resolution cut and source. However, I noticed that some of the data does not have a photo tree for every game (sample file #3 provided only has 8 of 9). I also tried using the "image" node as a key but I saw some anomalies in the data types that made the program crash.
