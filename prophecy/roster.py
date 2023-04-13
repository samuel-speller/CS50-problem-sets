from cs50 import SQL


#make a connection to the roster database
db = SQL('sqlite:///roster.db')

rows = db.execute("SELECT * FROM students")

for row in rows:
    db.execute('INSERT INTO Student_Names(Student_Name) VALUES(?)', row['student_name'])
    db.execute('INSERT INTO Houses(House_Name, House_Head) VALUES(?,?)', row['house'], row['head'])
    db.execute('INSERT INTO House_Assignments(Student_Name, House_Name) VALUES (?,?)', row['student_name'], row['house'])


