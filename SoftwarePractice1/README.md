Software Experiment 1 : Team project - Management

Purpose:
Make a program that manages data (membership, scores, statistics, etc.) on a variety of topics.

Submission:
1. Program descriptions (UML, important classes, methods, variables, etc.,)
2. Report (interface, function, action, discussion, etc.)

Requirements: The following must be fulfilled.

1. At least five data management functions must be available.
2. When the existing data is changed through the data management operation, the changed value should be maintained.
3. Be able to use the DB as needed. 4. The GUI is not available, and you should use the console.
5. Include the following functions.

Insert:
- The user enters three or more pieces of information according to the member's ID, Name, Sex, and Subject.
- If the id of the member you want to input already exists, the program should output an error message to the user.
- If the ID of the input member is not already input, the data is stored together with the notification of the new member.

Modify
- The user enters the member's ID.
- If the user fails to enter the member's ID, the program prints an error message and returns to the menu.
- If the ID of the user is already registered, all the information of the member is displayed.
- Afterwards, ask the user if the user wants to modify the information.
- If the user enters "yes", allows users to modify the information except ID, Name, and Sex.
- The contents are updated with the input data and stored.

Delete
- The user enters the member's ID.
- If the user fails to enter the member's ID, the program prints an error message and returns to the menu.
- If the ID of the user is already registered, all the information of the member is displayed.
- Afterwards, ask the user if the user wants to delete the information.
- If the user enters "yes", delete all information of the member.

Search
- If the program selects Find, the following menu is displayed: "Type? (All, ID, more than 3 items according to subject) "
- If the user selects "All", the program outputs the information for all members.
- If the user selects "ID", the user enters the member's ID, and the program outputs the information of the member matching the input ID. If the user enters the ID of invalid member, the program outputs an error message and returns to the menu.
- If the user selects "Item by topic", the user activates the function corresponding to the selected item. The user can use all three operators (> =, <=, ==).

Sort
- The user selects a sortable data item.
- The user can enter two sorting methods: Asc (ascending) or Desc (descending).
- The program outputs the information of all members including the data items entered in the ascending or descending order by the user.
- If the user enters invalid item, the program outputs the error message and re-print 