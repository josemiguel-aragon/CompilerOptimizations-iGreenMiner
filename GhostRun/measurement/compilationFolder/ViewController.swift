//
//  ViewController.swift
//  DBDemo
//
//  Created by Bilal Hassan on 23/5/19.
//  Copyright © 2019 Bilal Hassan. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource
{

    @IBOutlet weak var personTable: UITableView!

    let cellReuseIdentifier = "cell"

    var db:DBHelper = DBHelper()

    var persons:[Person] = []

    override func viewDidLoad() {
        super.viewDidLoad()

        personTable.register(UITableViewCell.self, forCellReuseIdentifier: cellReuseIdentifier)
        personTable.delegate = self
        personTable.dataSource = self

        //sleep(20)

        var currentDateTime=Date()
        var formatter=DateFormatter()
        formatter.timeStyle = .medium
        formatter.dateStyle = .long
        var dateTimeString=formatter.string(from: currentDateTime)
        
        //print("Time after calling view did load and before starting insertions: " + dateTimeString)
        NSLog("Time after calling view did load and before starting insertions: " + dateTimeString);
        
        fetchDataFromFirstAPI { [weak self] result in
            switch result {
            case .success(let firstData):
                
                for i in 1...1000 {
                    let randomInt = Int.random(in: 1..<2)
                    let n = "Bilal" + String(randomInt)
                    let a = randomInt
                    self?.db.insert(id: i, name: n, age: a)
                    let per = Person(id: i, name: n, age: a)
                    self?.persons.append(per)
                }
                
                    self?.fetchDataFromSecondAPI { result in
                        switch result {
                        case .success(let secondData):
                            print("Success!: \(secondData)")
                            currentDateTime=Date()
                            formatter=DateFormatter()
                           formatter.timeStyle = .medium
                           formatter.dateStyle = .long
                            dateTimeString=formatter.string(from: currentDateTime)
                           print("Time after insertions: " + dateTimeString)
                            self?.db.delete()
                            // Manejar los datos de la segunda solicitud
                        case .failure(let error):
                            print("Error: \(error.localizedDescription)")
                        }
                }
                
            case .failure(let error):
                print("Error: \(error.localizedDescription)")
            }
        }





        //sleep(20)
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
    
    // Método para la primera solicitud GET
    func fetchDataFromFirstAPI(completion: @escaping (Result<String, Error>) -> Void) {
        let urlString = "http://192.168.2.2:8089/startMeasurement"
        
        guard let url = URL(string: urlString) else {
            completion(.failure(NSError(domain: "URL no válida", code: 1, userInfo: nil)))
            return
        }
        
        let urlSession = URLSession.shared
        let task = urlSession.dataTask(with: url) { data, response, error in
            if let error = error {
                completion(.failure(error))
                return
            }
            
            guard let data = data, let dataString = String(data: data, encoding: .utf8) else {
                completion(.failure(NSError(domain: "Datos no válidos", code: 2, userInfo: nil)))
                return
            }
            
            completion(.success(dataString))
        }
        
        task.resume()
    }
    
    // Método para la segunda solicitud GET
    func fetchDataFromSecondAPI(completion: @escaping (Result<String, Error>) -> Void) {
        let urlString = "http://192.168.2.2:8089/endMeasurement"
        
        guard let url = URL(string: urlString) else {
            completion(.failure(NSError(domain: "URL no válida", code: 1, userInfo: nil)))
            return
        }
        
        let urlSession = URLSession.shared
        let task = urlSession.dataTask(with: url) { data, response, error in
            if let error = error {
                completion(.failure(error))
                return
            }
            
            guard let data = data, let dataString = String(data: data, encoding: .utf8) else {
                completion(.failure(NSError(domain: "Datos no válidos", code: 2, userInfo: nil)))
                return
            }
            
            completion(.success(dataString))
        }
        
        task.resume()
    }
}
