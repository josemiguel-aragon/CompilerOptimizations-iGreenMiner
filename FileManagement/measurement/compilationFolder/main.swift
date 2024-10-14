
//
//  AppDelegate.swift
//  DBDemo
//
//  Created by Bilal Hassan on 23/5/19.
//  Copyright © 2019 Bilal Hassan. All rights reserved.
//

import Foundation
import UIKit
import SQLite3

class AppDelegate: UIResponder, UIApplicationDelegate {
    var window: UIWindow?
}

UIApplicationMain(
    CommandLine.argc,
    UnsafeMutableRawPointer(CommandLine.unsafeArgv)
        .bindMemory(
            to: UnsafeMutablePointer<Int8>.self,
            capacity: Int(CommandLine.argc)),
    nil,
    NSStringFromClass(AppDelegate.self)
)

class Person
{
    
    var name: String = ""
    var age: Int = 0
    var id: Int = 0
    
    init(id:Int, name:String, age:Int)
    {
        self.id = id
        self.name = name
        self.age = age
    }
    
}


class DBHelper
{
    init()
    {
        db = openDatabase()
        createTable()
    }

    let dbPath: String = "myDb.sqlite"
    var db:OpaquePointer?

    func openDatabase() -> OpaquePointer?
    {
        let fileURL = try! FileManager.default.url(for: .documentDirectory, in: .userDomainMask, appropriateFor: nil, create: false)
            .appendingPathComponent(dbPath)
        var db: OpaquePointer? = nil
        if sqlite3_open(fileURL.path, &db) != SQLITE_OK
        {
            print("error opening database")
            return nil
        }
        else
        {
            print("Successfully opened connection to database at \(dbPath)")
            return db
        }
    }
    
    
    
    func createTable() {
        let createTableString = "CREATE TABLE IF NOT EXISTS person(Id INTEGER PRIMARY KEY,name TEXT,age INTEGER);"
        var createTableStatement: OpaquePointer? = nil
        if sqlite3_prepare_v2(db, createTableString, -1, &createTableStatement, nil) == SQLITE_OK
        {
            if sqlite3_step(createTableStatement) == SQLITE_DONE
            {
                print("person table created.")
            } else {
                print("person table could not be created.")
            }
        } else {
            print("CREATE TABLE statement could not be prepared.")
        }
        sqlite3_finalize(createTableStatement)
    }
    
    
    func insert(id:Int, name:String, age:Int)
    {
        /*let persons = read()
        for p in persons
        {
            if p.id == id
            {
                return
            }
        }*/
        let insertStatementString = "INSERT INTO person (Id, name, age) VALUES (NULL, ?, ?);"
        var insertStatement: OpaquePointer? = nil
        if sqlite3_prepare_v2(db, insertStatementString, -1, &insertStatement, nil) == SQLITE_OK {
            sqlite3_bind_text(insertStatement, 1, (name as NSString).utf8String, -1, nil)
            sqlite3_bind_int(insertStatement, 2, Int32(age))
            
            if sqlite3_step(insertStatement) == SQLITE_DONE {
                print(String(id) + " Successfully inserted row.")
            } else {
                print("Could not insert row.")
            }
        } else {
            print("INSERT statement could not be prepared.")
        }
        sqlite3_finalize(insertStatement)
    }
    
    func read() -> [Person] {
        let queryStatementString = "SELECT * FROM person;"
        var queryStatement: OpaquePointer? = nil
        var psns : [Person] = []
        if sqlite3_prepare_v2(db, queryStatementString, -1, &queryStatement, nil) == SQLITE_OK {
            while sqlite3_step(queryStatement) == SQLITE_ROW {
                let id = sqlite3_column_int(queryStatement, 0)
                let name = String(describing: String(cString: sqlite3_column_text(queryStatement, 1)))
                let year = sqlite3_column_int(queryStatement, 2)
                psns.append(Person(id: Int(id), name: name, age: Int(year)))
                print("Query Result:")
                print("\(id) | \(name) | \(year)")
            }
        } else {
            print("SELECT statement could not be prepared")
        }
        sqlite3_finalize(queryStatement)
        return psns
    }
    
    func update(_  localArr: inout [Person], id:Int, name:String, age:Int){
        
        for local in localArr{
            if local.id==id{
                local.name=name
                local.age=age
            }
        }
    }
    
    func deleteByID(id:Int) {
        let deleteStatementStirng = "DELETE FROM person WHERE Id = ?;"
        var deleteStatement: OpaquePointer? = nil
        if sqlite3_prepare_v2(db, deleteStatementStirng, -1, &deleteStatement, nil) == SQLITE_OK {
            sqlite3_bind_int(deleteStatement, 1, Int32(id))
            if sqlite3_step(deleteStatement) == SQLITE_DONE {
                print(String(id) + " Successfully deleted row.")
            } else {
                print("Could not delete row.")
            }
        } else {
            print("DELETE statement could not be prepared")
        }
        sqlite3_finalize(deleteStatement)
    }
    
    func delete() {
        let deleteTableString = "DROP TABLE IF EXISTS person;"
        var deleteTableStatement: OpaquePointer? = nil
        if sqlite3_prepare_v2(db, deleteTableString, -1, &deleteTableStatement, nil) == SQLITE_OK
        {
            if sqlite3_step(deleteTableStatement) == SQLITE_DONE
            {
                print("person table deleted.")
            } else {
                print("person table could not be deleted.")
            }
        } else {
            print("DROP TABLE statement could not be prepared.")
        }
        sqlite3_finalize(deleteTableStatement)
    }
    
}

class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource
{

    @IBOutlet weak var personTable: UITableView!

    let cellReuseIdentifier = "cell"

    var db:DBHelper = DBHelper()

    var persons:[Person] = []


    override func viewDidLoad() {
        super.viewDidLoad()

        DispatchQueue.main.async {
            self.personTable.register(UITableViewCell.self, forCellReuseIdentifier: self.cellReuseIdentifier)
            self.personTable.delegate = self
            self.personTable.dataSource = self

            //sleep(20)

            var currentDateTime=Date()
            var formatter=DateFormatter()
            formatter.timeStyle = .medium
            formatter.dateStyle = .long
            var dateTimeString=formatter.string(from: currentDateTime)

            //print("Time after calling view did load and before starting insertions: " + dateTimeString)
            NSLog("Time after calling view did load and before starting insertions: " + dateTimeString);


            for i in 1...10000 {
                let randomInt = Int.random(in: 1..<2)
                let n = "Bilal" + String(randomInt)
                let a = randomInt
                self.db.insert(id: i, name: n, age: a)
                let per = Person(id: i, name: n, age: a)
                self.personTable.reloadData()
                self.persons.append(per)
                main_all()

            }

            //sleep(1)

            self.db.delete()

            currentDateTime=Date()
            formatter=DateFormatter()
           formatter.timeStyle = .medium
           formatter.dateStyle = .long
            dateTimeString=formatter.string(from: currentDateTime)
           print("Time after insertions: " + dateTimeString)
           if let url = URL(string: "launching://"){
                if UIApplication.shared.canOpenURL(url){
                    UIApplication.shared.open(url, options: [:], completionHandler: {_ in
                        exit(0)
                    })

                } else{
                    print("Unable to open the app.")
                }
           }
       }
    }

    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int
    {
        return persons.count
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell
    {

        let cell:UITableViewCell = tableView.dequeueReusableCell(withIdentifier: cellReuseIdentifier)!


        cell.textLabel?.text = "Id: " + String(persons[indexPath.row].id) + ", " + "Name: " + persons[indexPath.row].name + ", " + "Age: " + String(persons[indexPath.row].age)

        return cell
    }
}
