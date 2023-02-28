## Students
- Jeevan Sivamohan, WUSTL ID: 498510
- Zachary Bertino, WUSTL ID: 498237

## Links
- [File Sharing Site](http://ec2-54-164-69-76.compute-1.amazonaws.com/~zachBert/module_3_group/News_page.php?) - Link to our news site

## Creative Portion
Our creative portion of the project comprises of 4 main additions in functionality to the website

- We added a page to display all users. This page not only displays usernames, but also the number of stories and comments that they have created.

- We added the ability to delete a user. Doing so means that no one can login as that username and password. No stories or comments are deleted, but instead they show as written by "User Unknown." In addition, that user no longer appears in display all users.

- We added a like feature that was stored in a new column of the stories database. Liking a story will run a php file that will first return the current number of likes the story has and then update it so that the number of likes for the story is one more. In addition, when stories are displayed, they are shown in order of decreasing likes, meaning that more popular stories will be at the top.

- We added a feature to search through stories for a keyword. First, all titles are searched and displayed in order of likes if they contain the keyword. Then the bodies of the remaining stories are searched and displayed in order of likes under those. (For a good example, search for the keyword "day")

## Notes

- Duplicate usernames are not allowed

- Not all stories have links attatched! If they do, you can find them labeled "link to relevant info."

