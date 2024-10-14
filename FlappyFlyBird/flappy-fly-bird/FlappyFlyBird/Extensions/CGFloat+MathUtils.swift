//
//  Float+MathUtils.swift
//  FlappyFlyBird
//
//  Created by Astemir Eleev on 02/05/2018.
//  Copyright © 2018 Astemir Eleev. All rights reserved.
//

import CoreGraphics
import Foundation

struct SeededGenerator: RandomNumberGenerator {
    private var state: UInt64
    
    init(seed: UInt64) {
        self.state = seed
    }
    
    mutating func next() -> UInt64 {
        // Parámetros del LCG
        let a: UInt64 = 6364136223846793005
        let c: UInt64 = 1
        state = a &* state &+ c
        return state
    }
}

var generator = SeededGenerator(seed:0)

extension CGFloat {
    
    // MARK: - Properties
    var toRadians: CGFloat {
        return CGFloat.pi * self / 180
    }
    
    // MARK: - Methods
    
    func clamp(min: CGFloat, max: CGFloat) -> CGFloat {
        if (self > max) {
            return max
        } else if (self < min) {
            return min
        } else {
            return self
        }
    }
    
    static func range(min: CGFloat, max: CGFloat) -> CGFloat {
        CGFloat.random(in: min...max, using: &generator)
    }
}
