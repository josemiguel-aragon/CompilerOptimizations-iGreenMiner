//
//  ViewController.swift
//  testMeasurementServer
//
//  Created by José Miguel Aragón Jurado on 21/5/24.
//

import UIKit

class ViewController: UIViewController {
    var count = 0

    override func viewDidLoad() {
        super.viewDidLoad()
    }

    @IBAction func fetchData(_ sender: UIButton) {
            fetchDataFromFirstAPI { [weak self] result in
                switch result {
                case .success(let firstData):
                    // Procesar los datos de la primera solicitud si es necesario y luego hacer la segunda solicitud
                    let seconds = 1.0
                    DispatchQueue.main.asyncAfter(deadline: .now() + seconds) {
                        self?.fetchDataFromSecondAPI { result in
                            switch result {
                            case .success(let secondData):
                                self?.count+=1
                                if self?.count ?? 1000 < 1000{
                                    self?.fetchData(sender)
                                }
                                print("Segunda solicitud completada: \(secondData)")
                                // Manejar los datos de la segunda solicitud
                            case .failure(let error):
                                print("Error en la segunda solicitud: \(error.localizedDescription)")
                            }
                        }
                    }
                    
                case .failure(let error):
                    print("Error en la primera solicitud: \(error.localizedDescription)")
                }
            }

    }
    
    // Método para la primera solicitud GET
    func fetchDataFromFirstAPI(completion: @escaping (Result<String, Error>) -> Void) {
        let urlString = "http://192.168.1.101:8089/startMeasurement"
        
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
        let urlString = "http://192.168.1.101:8089/endMeasurement"
        
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
