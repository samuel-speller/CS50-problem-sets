CREATE TABLE Student_Names (
    Student_id INTEGER PRIMARY KEY,
    Student_Name text NOT NULL
);

CREATE TABLE Houses (
    House_id INTEGER PRIMARY KEY,
    House_Name text NOT NULL,
    House_Head text NOT NULL,
    FOREIGN KEY (House_id) REFERENCES Student_Names(Student_id)
);

CREATE TABLE House_Assignments (
    Assignment_id INTEGER PRIMARY KEY,
    Student_Name text NOT NULL,
    House_Name text NOT NULL,
    FOREIGN KEY (Assignment_id) REFERENCES Student_Names(Student_id)
);
