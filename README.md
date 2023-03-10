# Word Hunt Solver (for GamePigeon iMessage Word Hunt Game)
# ⭐ Introduction ⭐
I've always wanted to beat my friends in Word Hunt, so I built a solver that gives me all possible solutions given a board of letters. This program employs a recursive search algorithm that looks for words in all directions including words that do not have a straight path.

# 💥 Demo 💥
![ezgif com-optimize](https://user-images.githubusercontent.com/95982168/221720628-d6f84277-52ca-495b-9364-3d96b9e0d5c2.gif)

Board input file:

<img width="152" alt="image" src="https://user-images.githubusercontent.com/95982168/221717785-c4d25a57-56bf-401b-b186-26e83924fc4a.png">

Output:

<img width="350" alt="image" src="https://user-images.githubusercontent.com/95982168/221717858-50f5625d-d3ba-41fb-bba3-5d8be85754d7.png"> <img width="320" alt="image" src="https://user-images.githubusercontent.com/95982168/221724066-f48ff66a-a6c0-4e53-8b91-53c6fc8ec62b.png">




# 🥸 Specification 🥸
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

# 👻 Side Notes 👻
- This project indirectly made my Word Search Solver Project redundant due to the Word Hunt Solver did not need a specified work bank. In addition, the user can specify the -l options to output linear solutions only, which is ideal for word searches. In testing, I found that Word Hunt Solver runs circles around Word Search Solver in terms of runtime. At the time, I did not take my Data Structures and Algorithms class when I made Word Search Solver, but currently at the time of writing this, I am in Data Structures and Algorithms. It really helped! 

Example: Using Chinese New Year Themed Word Search
Command Line Arguments: -c -w -p -l -b chinese_new_year_board.txt -s 20>output.txt

<img width="362" alt="image" src="https://user-images.githubusercontent.com/95982168/221723585-8f7ccda1-9850-4603-849c-dd0a372a7cd1.png"> <img width="357" alt="image" src="https://user-images.githubusercontent.com/95982168/221723656-f2c03163-277a-4253-bf95-c993b294ba83.png"> <img width="265" alt="image" src="https://user-images.githubusercontent.com/95982168/221723990-ae84c57b-23c9-4f32-b73a-dcdb2c7c0340.png">


