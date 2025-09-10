import os

class Student:
    def __init__(self, IDNo=0, name="", cgpa=0.0, email="", phone="", course="", semester=0):
        self.IDNo = IDNo
        self.name = name
        self.cgpa = cgpa
        self.email = email
        self.phone = phone
        self.course = course
        self.semester = semester

    def input_data(self):
        self.name = input("Name: ")
        self.cgpa = float(input("CGPA: "))
        self.email = input("Email: ")
        self.phone = input("Phone: ")
        self.course = input("Course: ")
        self.semester = int(input("Semester: "))

    def display(self, detailed=False):
        if detailed:
            print(f"\nID No: {self.IDNo}")
            print(f"Name: {self.name}")
            print(f"CGPA: {self.cgpa}")
            print(f"Email: {self.email}")
            print(f"Phone: {self.phone}")
            print(f"Course: {self.course}")
            print(f"Semester: {self.semester}\n")
        else:
            print(f"{self.IDNo:<10}{self.name:<20}{self.cgpa:<10}")

    def to_list(self):
        return [str(self.IDNo), self.name, str(self.cgpa), self.email, self.phone, self.course, str(self.semester)]

    @staticmethod
    def from_list(data):
        return Student(int(data[0]), data[1], float(data[2]), data[3], data[4], data[5], int(data[6]))


class StudentManager:
    def __init__(self, filename="students.txt"):
        self.filename = filename

    def exists(self, IDNo):
        for student in self.load_students():
            if student.IDNo == IDNo:
                return True
        return False

    def load_students(self):
        students = []
        if os.path.exists(self.filename):
            with open(self.filename, "r") as f:
                lines = [line.strip() for line in f.readlines()]
                for i in range(0, len(lines), 7):
                    students.append(Student.from_list(lines[i:i+7]))
        return students

    def save_students(self, students):
        with open(self.filename, "w") as f:
            for s in students:
                f.write("\n".join(s.to_list()) + "\n")

    def add_student(self):
        IDNo = int(input("ID No: "))
        if self.exists(IDNo):
            print("Student exists!")
            return
        s = Student(IDNo)
        s.input_data()
        students = self.load_students()
        students.append(s)
        self.save_students(students)
        print("Student added successfully!")

    def display_all(self):
        students = self.load_students()
        print(f"\n{'IDNo':<10}{'Name':<20}{'CGPA':<10}")
        print("-"*40)
        for s in students:
            s.display()

    def search_by_ID(self):
        IDNo = int(input("Enter Roll No: "))
        students = self.load_students()
        found = False
        for s in students:
            if s.IDNo == IDNo:
                s.display(detailed=True)
                found = True
                break
        if not found:
            print("Not found!")

    def search_by_name(self):
        name = input("Enter Name: ")
        students = self.load_students()
        found = False
        for s in students:
            if s.name.lower() == name.lower():
                s.display(detailed=True)
                found = True
        if not found:
            print("Not found!")

    def update_student(self):
        IDNo = int(input("Enter Roll No to update: "))
        students = self.load_students()
        found = False
        for i, s in enumerate(students):
            if s.IDNo == IDNo:
                print("Enter new data:")
                s.input_data()
                students[i] = s
                found = True
                break
        self.save_students(students)
        print("Updated successfully!" if found else "Student not found!")

    def delete_student(self):
        IDNo = int(input("Enter Roll No to delete: "))
        students = self.load_students()
        students = [s for s in students if s.IDNo != IDNo]
        self.save_students(students)
        print("Deleted successfully!")

    def show_statistics(self):
        students = self.load_students()
        if not students:
            print("No records!")
            return
        total = sum(s.cgpa for s in students)
        high = max(s.cgpa for s in students)
        low = min(s.cgpa for s in students)
        print(f"\nTotal Students: {len(students)}")
        print(f"Average CGPA: {total/len(students):.2f}")
        print(f"Highest CGPA: {high}")
        print(f"Lowest CGPA: {low}\n")


def menu():
    print("\n1. Add Student\n2. Display All\n3. Search by Roll\n4. Search by Name\n"
          "5. Update Student\n6. Delete Student\n7. Statistics\n8. Exit")
    return int(input("Choice: "))

def main():
    sm = StudentManager()
    while True:
        choice = menu()
        if choice == 1:
            sm.add_student()
        elif choice == 2:
            sm.display_all()
        elif choice == 3:
            sm.search_by_ID()
        elif choice == 4:
            sm.search_by_name()
        elif choice == 5:
            sm.update_student()
        elif choice == 6:
            sm.delete_student()
        elif choice == 7:
            sm.show_statistics()
        elif choice == 8:
            print("System Ended!")
            break
        else:
            print("Invalid choice!")


if __name__ == "__main__":
    main()
