# COVID-system
COVID-19 system, description in REDME
SpreaderDetectorBackend
-----------------------------
Please explain how you dealt with the following parts of the exam.

Input processing
----------------
What I did with the people.in file was: I went over the file line by line in the fileparser 
method and stored the age, name, and ID of each person into a struct (person) which contains 
those fields and also a field which contains the probability that the person caught the disease. 

For the Meetings.in file- I went over the file line for line in MeetingsParser and updated the 
probability that a person caught the disease using the crna function and the spreader's probability.

Data storing
------------
I stored each person in a struct to unify all of the information.
I stored the structs into a struct array (people*).
the reason was- I wanted to store it in a way where I can use binary search to find a person 
in O(log(n)).


Results sorting
---------------
after retrieving the information from the people.in file, I sorted the people array in O(nlog(n)) by ID using 
the quicksort algorithem, that way- I can retrieve a person in O(log(n)).
After updating all the peoples probability- I sort the people array by probability and then I print all of the 
relevant information.
therefore- my program runs in O(nlog(n)) if "n" is the 
max{number of lines in people.in, number of lines in meetings.in}.
