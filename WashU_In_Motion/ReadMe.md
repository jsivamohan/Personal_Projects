## Introduction
This was is a project I am currently taking on with the end goal of creating a density map to visualize the movement of students over the Washington University in St. Louis campus over time. 

## Web Scraper
This was done by building a webscraper using Selenium and BeautifulSoup in order to collect data from the [course listings](https://courses.wustl.edu/Semester/Listing.aspx) website of the number of students in each class offered by the university along with which building and room the class is in as well as the day and time of the class. All of this information was then written into a csv file so that it could be analyzed.

## Jupyter Notebook
Once all of the information was written to a csv file, it was analyzed in a jupyter notebook using python pandas, numpy, and matplotlib. Significant cleaning was needed in order to convert all of the information gathered from the website including string manipulation, one hot encoding, and removal of null values.

Graphs were created in order to visualize the dispersion of students in buildings through the day. Analysis was also done into the geographic location of different locations on campus using the longitude and latitude of each building on campus. The geopy library was used in order to calculate the distance between each building and each of the dining locations on campus. This information combined with the opening and closing hours of each, along with the number of students that get out of class from each building, was used in order to make a function to predict the number of students in each of the dining halls on campus as a function of the number of students that accounts for the distance to each dining location and the time since they got out of class.

## Database
Once all of the necessary information was collected into dataframes, it was written to a csv file which was then uploaded to databases stored in an AWS instance for future implementation on a public domain website.

## File Information
All of the txt files are CSV files that have the information scraped from the course [course listings](https://courses.wustl.edu/Semester/Listing.aspx) or written from the jupyter notebook file to be stored in a SQL database on my AWS instance. The actual code for the webscraper itself is stored in webstac_scraper.py. While the data analysis and visualization was done in a jupyter notebook called Webstac_info_analysis.ipynb.

