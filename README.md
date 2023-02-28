# Word Hunt Solver (for GamePigeon iMessage Word Hunt Game)
# ⭐ Introduction ⭐
I've always wanted to beat my friends in Word Hunt, so I built a solver that gives me all possible solutions given a board of letters. This program employs a recursive search algorithm that looks for words in all directions including words that do not have a straight path.

# 💥 Demo 💥
Board input file:

<img width="163" alt="image" src="https://user-images.githubusercontent.com/95982168/221713779-ffd2730e-8b3a-4131-8fa8-7bc8eddeaec8.png">

Output:

<img width="272" alt="image" src="https://user-images.githubusercontent.com/95982168/221714134-7ab55004-af98-44e0-9cd1-644e1b6c9a30.png">

# 👓 Specification 👓
This program uses many command line options. Each command line option is thoroughly discussed here:

Cardinal Output -c (Recommended): Displays found word's starting position, then traces out the found word using cardinal directions.

Index Output -i: Displays found word's starting position, then traces out the found word using row and column coordinates.

Point Output -p (Recommended): Prioritizes found words that would yield the most points and outputs them first. 

Alpha Output -a: Displays found words in alphabetical order.

Search Depth -s (Fun to play around with, but nor required): Requires an argument that specifies search depth of the search algorithm. For example, if a given board obtains a possible traceable solution “APPLES” and the search depth specified is 5, then “APPLE” is solution that is outputted, but “APPLES” is not a solution that is outputted, since “APPLES” contains 6 letters. Generally, the higher the search depth, the longer the runtime albeit not by a significant amount. Testing shows that runtime will generally depend on search depth and size of board. 

Board -b (required): Requires argument that specifies what board file the Word Hunt Solver should read.

Linear Search Only -l (lowercase L): If specified, only solutions that are linear are outputted.
Example:

<img width="152" alt="image" src="https://user-images.githubusercontent.com/95982168/221717506-08920059-7ff5-47f0-aefb-e456740c9ee1.png">

"ANT" is a linear solution because the solution's trace is straight but "DENT" is not a linear solution because the solution's trace is not straight. If -l is specified, then only "ANT" will be outputted as a solution. "DENT" is not considered to be a solution.

Word Hunt Mode -w (required for Word Hunt Game): If specified, only solutions that are valid in Word Hunt for GamePigeon are outputted.

Specify Length Mode -x: If specified, requires an argument that is an integer. Only solutions that are of length that is specified is outputted.

Specify First Letter Mode -y: If specified, requires an argument that is a string of letters. Only words that alphabetically start with these specified letters will be outputted. 


