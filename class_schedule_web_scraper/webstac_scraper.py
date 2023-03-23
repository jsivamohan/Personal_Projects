from bs4 import BeautifulSoup
import requests
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from webdriver_manager.chrome import ChromeDriverManager
from selenium.common.exceptions import StaleElementReferenceException
from selenium.common.exceptions import NoSuchElementException
import time



#start active

options = Options()
# options.add_experimental_option("detach", True)

driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()))

def extract_info(dept, id_ref=None):
    # driver.get("https://courses.wustl.edu/Semester/Listing.aspx")
    test_id = ""
    dept_name = ""
    links = driver.find_elements("xpath", "//a[@href]")
    AS_links.clear()
    # sem_links.clear()
    for link in links:
        # if "Body_hlSemester4" in link.get_attribute("id"):
        #     sem_links.append(link)
        if "Body_dlDepartments" in link.get_attribute("id"):
            AS_links.append(link)
    # sem_links[0].click()
    AS_links[dept].click()
    dept_name = AS_links[dept].text
    print("dept is " , dept)
    if(id_ref != None): 
        while(True):
            print("checking in loop again")
            try:
                # poll the link with an arbitrary call
                driver.find_element(By.ID, id_ref)
            except NoSuchElementException or StaleElementReferenceException:
                soup = BeautifulSoup(driver.page_source, 'html.parser')
                lists = soup.find_all('table', class_="MainTableRow SecOpen")
                break
    else:#on the first iteration you just check if maintablerow is empty or not
        while(True):
            print("checking in loop")
            soup = BeautifulSoup(driver.page_source, 'html.parser')
            lists = soup.find_all('table', class_="MainTableRow SecOpen")
            print("list length " + str(len(lists)))
            if(len(lists)!=0):
                break
            lists = soup.find_all('table', class_="MainTableRow SecClosed")
            if(len(lists)!=0):
                break
    info_list = []
    # soup = BeautifulSoup(driver.page_source, 'html_parser')
    # lists = soup.find_all('table', class_="MainTableRow SecOpen")
    if(len(lists)==0):
        return None
    id_ref = lists[0].get("id")
    print("test_id: " , id_ref)
    print("open list : " + str(len(lists)))
    new_name = dept_name.replace(",", "")
    for list in lists:
        base_info = list.find_all('td', class_="ItemRow")
        seat_info = list.find_all('td', class_="ItemRowCenter")
        info_list.append((base_info[1].text.strip(), base_info[2].text.strip(), base_info[3].text.strip(), seat_info[0].text.strip(),seat_info[1].text.strip(), seat_info[2].text.strip(), new_name.strip()))
    lists = soup.find_all('table', class_="MainTableRow SecClosed")
    print("closed list : " + str(len(lists)))
    for list in lists:
        base_info = list.find_all('td', class_="ItemRow")
        seat_info = list.find_all('td', class_="ItemRowCenter")
        info_list.append((base_info[1].text.strip(), base_info[2].text.strip(), base_info[3].text.strip(), seat_info[0].text.strip(), seat_info[1].text.strip(), seat_info[2].text.strip(), new_name.strip()))
    f = open("new_test_file.txt", "a")
    # f.write("more testing")
    for c in info_list:
        for info in c:
            # print(info)
            f.write(str(info) + ", ")
            f.flush()
        f.write("\n")
    f.close()
    return (info_list, id_ref)


AS_links = []

main_info_list = []#list of all of the departments in art_sci and their information

test_id  = ""#string of id on page for table used to check if we're now on a new table

sem_links= []

driver.get("https://courses.wustl.edu/Semester/Listing.aspx")
links = driver.find_elements("xpath", "//a[@href]")
for link in links:
    if "Body_hlSemester4" in link.get_attribute("id"):
        sem_links.append(link)
sem_links[0].click()

test_compare = "font-weight: bold;"
test_case_string = ""
check_list = []
time.sleep(2)
while(True):
    try:
        # elems = driver.find_elements("xpath", "//a[(@id = 'Body_hlSemester4') and (@style = 'font-weight: normal;')]")
        # elems = driver.find_elements(By.ID, 'Body_hlSemester4')
        # print("length init: " ,len(elems))
        # print(elems[0].get_attribute("style"))
        # for e in elems:
        if "bold" in driver.find_elements(By.ID, 'Body_hlSemester4')[0].get_attribute("style"):
            break
        # print(driver.find_element(By.ID, 'Body_hlSemester4').get_attribute("style"))
        # print(len(elems))
        # if(len(elems)==0):
        #     break
    except StaleElementReferenceException:
        break
print(driver.find_element(By.ID, 'Body_hlSemester4').get_attribute("style"))
num_dept = 0
school_links = []

links = driver.find_elements("xpath", "//a[@href]")
for link in links:
    if "Body_repSchools" in link.get_attribute("id"):
        school_links.append(link)

school_links[10].click()
time.sleep(2)
while(True):
    try:
        if "bold" in driver.find_elements(By.ID, 'Body_repSchools_lnkSchool_10')[0].get_attribute("style"):
            break
    except StaleElementReferenceException:
        break

links = driver.find_elements("xpath", "//a[@href]")
for link in links:
    if "Body_dlDepartments" in link.get_attribute("id"):
        num_dept = num_dept +1



# extract_info(68,None)
#keep for normal use
test_id = None
for i in range(1,num_dept):
    print("i is: " ,i)
    ret_tuple = extract_info(i, test_id)
    if ret_tuple is not None:
        print("new_id")
        test_id = ret_tuple[1]
    # main_info_list.append(ret_tuple[0])
#161
#end active
