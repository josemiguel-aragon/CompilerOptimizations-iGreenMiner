//
//  ViewController.swift
//  FileManagement_Practice
//
//  Created by PCS213 on 30/11/22.
//

import UIKit

class ViewController: UIViewController {

    // Outlet
    @IBOutlet weak var assistanceImage: UIImageView!


    // Variable
    let fm = LocalFileManager.instance
    let imageName = "spidy"

    override func viewDidLoad() {
        super.viewDidLoad()

        DispatchQueue.main.async {
            for _ in 1...1000{
                self.fm.saveToFileManager(image: UIImage(named: self.imageName), name: self.imageName)
                //self.setImage()
                self.fm.deleteFromFileManager(name: self.imageName)
            }

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


     @IBAction private func didTapOnSaveToFmButton(_ sender: UIButton) {

        fm.saveToFileManager(image: UIImage(named: imageName), name: imageName)
        //fm.savePdfIntoFileManager(urlString: "file:///Users/pcs213/Downloads/kemh1dd/kemh1a1.pdf")
    }

    @IBAction func didTapOnSetImageButton(_ sender: UIButton) {
        setImage()
    }

    @IBAction private func didTapOnDeleteFmButton(_ sender: UIButton) {
        fm.deleteFromFileManager(name: imageName)

    }

    func setImage() {
        guard let path = fm.getFromFilePath(name: imageName) else {
            return
        }
        let data = try? Data(contentsOf: path)
        assistanceImage.image = UIImage(data: data!)
        print(path)
    }
}


class LocalFileManager {
    static let instance = LocalFileManager()

    let manager = FileManager.default
    let folderName = "MyFolder"

    init() {
        createFolderIfNeeded()
    }

//MARK: Function to create Folder

    func createFolderIfNeeded() {
        guard
            let path = manager.urls(for: .cachesDirectory, in: .userDomainMask).first?.appendingPathComponent(folderName).path else {
                return
            }

        if !FileManager.default.fileExists(atPath: path){
            do {
                try FileManager.default.createDirectory(atPath: path, withIntermediateDirectories: true, attributes: nil)
            } catch let error {
                print("Error \(error)")
            }
        }
    }

//MARK: Function to delete Folder

    func deleteFolder() {
        guard
            let path = manager.urls(for: .cachesDirectory, in: .userDomainMask).first?.appendingPathComponent(folderName).path else {
                return
            }

        do {
            try FileManager.default.removeItem(atPath: path)
            print("Success in deleting folder")
        }
        catch let error {
            print("Error in deleting folder \(error)")
        }
    }

// MARK: -> Function to save data into FileManager

    func saveToFileManager(image: UIImage?, name: String) {
        guard
            let data = image?.jpegData(compressionQuality: 1.0),
            let path = getFromFilePath(name: name)
        else {
              print("Error in loading data")
              return
        }

        print(path)

        do {
            try data.write(to: path)
            print("Image Saved")
        } catch let error {
            print("Error \(error)")
        }
        saveTextFile()
    }



//MARK: -> Function to get data from Filemanger
    func getFromFilePath(name: String) -> URL? {
        guard let path = FileManager.default.urls(for: .cachesDirectory, in: .userDomainMask).first?.appendingPathComponent(folderName).appendingPathComponent("\(name).jpeg") else {
            print("Error getting file path")
            return nil
        }
        return path
    }

//MARK: -> Function to delete data from FileManager
    func deleteFromFileManager(name: String) {
        guard let path = getFromFilePath(name: name),
              FileManager.default.fileExists(atPath: path.path) else {
            print("Error int getting file path")
            return
        }

        do {
            try FileManager.default.removeItem(at: path)
            print("...Deleting Image...")
        }
        catch let error {
            print("Error \(error)")
        }
    }

//MARK: -> Function to save TextFile
    func saveTextFile() {
        let file = "sum.text"
        let text = "2 + 2"
        guard let url = manager.urls(for: .cachesDirectory, in: .userDomainMask).first?.appendingPathComponent(folderName) else {
            print("Error in getting file path")
            return
        }
        let path = url.appendingPathComponent(file)

        do {
            try text.write(to: path, atomically: false, encoding: .utf8)
        }
        catch let error {
            print("Error in saving textFile \(error)")
        }
    }


//MARK: -> Function to save pdf file
    func savePdfIntoFileManager(urlString: String) {
        DispatchQueue.main.async {
            let url = URL(string: urlString)
            let pdfData = try? Data.init(contentsOf: url!)

            guard let path = self.manager.urls(for: .cachesDirectory, in: .userDomainMask).first?.appendingPathComponent(self.folderName) else {
                print("Error in getting PDF path")
                return
            }

            let actualPath = path.appendingPathComponent("Maths.pdf")

            do {
                try pdfData?.write(to: actualPath, options: .atomic)
                print("PDF Added successfully")
            }
            catch let error {
                print("Error \(error)")
            }
        }
    }
}
