//
//  ViewController.swift
//  SimpleClassificationApp
//
//  Created by GwakDoyoung on 11/11/2018.
//  Copyright © 2018 tucan9389. All rights reserved.
//

import UIKit
import Vision

class ViewController: UIViewController {
    @IBOutlet weak var imageView: UIImageView!
    @IBOutlet weak var label: UILabel!

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        DispatchQueue.main.async {
            self.testModel()


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

    func testModel() {


            let fileManager = FileManager.default
            let bundleURL = Bundle.main.bundleURL
            let assetURL = bundleURL.appendingPathComponent("dataset.bundle")

            do {
                let contents = try fileManager.contentsOfDirectory(at: assetURL, includingPropertiesForKeys: [URLResourceKey.nameKey, URLResourceKey.isDirectoryKey], options: .skipsHiddenFiles)
                for i in 1...10{
                    var count = 0
                    for item in contents
                    {
                        count+=1

                            NSLog(String(count))
                            NSLog(item.lastPathComponent)
                            self.loadImageAndPredict(lastPathComponent: item.lastPathComponent)
                           //usleep(10000)

                    }
                }
            }

            catch let error as NSError {
                print(error)
            }
        }



    func loadImageAndPredict(lastPathComponent: String){

            let bundlePath = Bundle.main.path(forResource:"dataset.bundle/" + lastPathComponent.components(separatedBy:".")[0], ofType: lastPathComponent.components(separatedBy:".")[1]) ?? ""
            guard  let image: UIImage = UIImage(contentsOfFile: bundlePath),
                   let pixelBuffer: CVPixelBuffer = image.pixelBuffer(width: Int(image.size.width),
                                                                      height: Int(image.size.height))
            else{
                return
            }




            //self.imageView.image=image


            // create model
            var model = ImageClassifier()

            // predict
            if let result = try? model.prediction(image: pixelBuffer) {
                let predictedLabel = result.classLabel
                let confidence = result.classLabelProbs[result.classLabel] ?? 0.0
                //self.label.text = "\(predictedLabel), \(confidence)"

                NSLog("\(predictedLabel), \(confidence)")
            }
        }
}

