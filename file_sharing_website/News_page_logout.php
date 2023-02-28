<?php
//simple logout to just set the $_SESSION['user_id'] to -1 and go back to the news page
session_start();
$_SESSION['user_id'] = -1;
$_SESSION['user_name'] = "user";
echo "got to logout";
header('Location: News_page.php');
exit;
?>