//
//  GameScene.swift
//  ghost-run
//
//  Created by Kunz, Gabriel on 03/10/20.
//

import SpriteKit
import GameplayKit

class GameScene: SKScene {
    
    // Nodes
    var gameNode: SKNode!
    var playerNode: SKNode!
    var groundNode: SKNode!
    var backgroundNode: SKNode!
    var obstacleNode: SKNode!
    private var exitFrames: Int = 0
    
    // Background
    var backgroundImage = SKSpriteNode(imageNamed: "game.assets/background")
    
    // Sprites
    var playerSprite: SKSpriteNode!
    
    // Score
    var scoreNode: SKLabelNode!
    var score = 0 as Int
    
    // Consts
    let background = 0 as CGFloat
    let foreground = 1 as CGFloat
    
    // Ground variables
    var groundHeight: CGFloat?
    var groundSpeed = 500 as CGFloat
    
    // Player variables
    var playerYPosition: CGFloat?
    var playerYPositionMultiplier = 0.4 as CGFloat
    
    
    override func didMove(to view: SKView) {
        // Background
        backgroundImage.position = CGPoint(x: frame.size.width / 2, y: frame.size.height / 2)
        backgroundImage.zPosition = background
        addChild(backgroundImage)
        
        // Ground
        groundNode = SKNode()
        groundNode.zPosition = foreground
        createGround()
        
        // Player
        playerNode = SKNode()
        playerNode.zPosition = foreground
        createPlayer()
        
        gameNode = SKNode()
        gameNode.addChild(playerNode)
        gameNode.addChild(groundNode)
        self.addChild(gameNode)
    }
    
    func createPlayer(){
        let screenWidth = self.frame.size.width
        let screeHeight = self.frame.size.height
        let playerScale = 0.3 as CGFloat
        // Player texture
        let playerTexture1 = SKTexture(imageNamed: "game.assets/floating/1")
        let playerTexture2 = SKTexture(imageNamed: "game.assets/floating/2")
        let playerTexture3 = SKTexture(imageNamed: "game.assets/floating/3")
        let playerTexture4 = SKTexture(imageNamed: "game.assets/floating/4")
        let playerTexture5 = SKTexture(imageNamed: "game.assets/floating/5")
        playerTexture1.filteringMode = .nearest
        playerTexture2.filteringMode = .nearest
        playerTexture3.filteringMode = .nearest
        playerTexture4.filteringMode = .nearest
        playerTexture5.filteringMode = .nearest
        
        // Animation
        let floatingAnimation = SKAction.animate(with: [playerTexture1, playerTexture2, playerTexture3, playerTexture4, playerTexture5], timePerFrame: 0.06)
        
        for i in 1...10000{
            // Size and scale
            playerSprite = SKSpriteNode()
            playerSprite.size = playerTexture1.size()
            playerSprite.setScale(playerScale)
            playerNode.addChild(playerSprite)
            
            // Positioning
            playerSprite.position = CGPoint(x: screenWidth * 0.15, y: screeHeight * 0.435)
            playerSprite.run(SKAction.repeatForever(floatingAnimation))
        }
    }
    
    func createGround(){
        let screenWidth = self.frame.size.width
        
        // Ground texture
        let groundTexture = SKTexture(imageNamed: "game.assets/ground")
        groundTexture.filteringMode = .nearest
        
        // Ground size
        let homeButtonPadding = 50.0 as CGFloat
        groundHeight = groundTexture.size().height*1.4 + homeButtonPadding
        
        // Ground movement
        let moveGroundLeft = SKAction.moveBy(x: -groundTexture.size().width, y: 0.0, duration: TimeInterval(screenWidth /    groundSpeed))
        let resetGround = SKAction.moveBy(x: groundTexture.size().width, y: 0.0, duration: 0.0)
        let groundMoveLoop = SKAction.sequence([moveGroundLeft,resetGround])
        
        // Node
        let numberOfGroundNodes = 1 + Int(ceil(screenWidth / groundTexture.size().width))
        
        // Ground position and movement loop
        for i in 0 ..< numberOfGroundNodes{
            let node = SKSpriteNode(texture: groundTexture)
            
            node.anchorPoint = CGPoint(x: 0.0, y: 0.0)
            node.position = CGPoint(x: CGFloat(i) * groundTexture.size().width, y: groundHeight!)
            groundNode.addChild(node)
            node.run(SKAction.repeatForever(groundMoveLoop))
        }
        
    }
    
    override func update(_ currentTime: TimeInterval) {
        super.update(currentTime)
        exitFrames+=1
        
        if exitFrames >= 3600{
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
    
}
