## Introduction
This was project I took on with a small group to create a density map to visualize the movement of students over the Washington University in St. Louis campus over time. 

## Web Scraper
This was done by building a webscraper using Selenium and BeautifulSoup in order to collect data from the [course listings](https://courses.wustl.edu/Semester/Listing.aspx) website of the number of students in each class offered by the university along with which building and room the class is in as well as the day and time of the class. All of this information was then written into a csv file so that it could be analyzed.

## Jupyter Notebook
Once all of the information was written to a csv file, it was analyzed in a jupyter notebook using python pandas, numpy, and matplotlib. Significant cleaning was needed in order to convert all of the information gathered from the website including string manipulation, one hot encoding, and removal of null values.

Graphs were created in order to visualize the dispersion of students in buildings through the day. Analysis was also done into the geographic location of different locations on campus using the longitude and latitude of each building on campus. The geopy library was used in order to calculate the distance between each building and each of the dining locations on campus. This information combined with the opening and closing hours of each, along with the number of students that get out of class from each building.

## Front End
All of the information that was scraped and cleaned from the course listings was then given to the front end through csv files. The front end was built using d3 visualization. The campus map itself was constructed as an SVG file and d3 was used in order to correlate each building with it's location on the map, allow colors to change over time to reflect changes in population density, and gives the user the option to click on a building to see the classes being taught there at any point in time.

## File Information
All of the txt files are CSV files that have the information scraped from the course [course listings](https://courses.wustl.edu/Semester/Listing.aspx) or written from the jupyter notebook file to be stored in a SQL database on my AWS instance. The actual code for the webscraper itself is stored in webstac_scraper.py. While the data analysis and visualization was done in a jupyter notebook called Webstac_info_analysis.ipynb.
