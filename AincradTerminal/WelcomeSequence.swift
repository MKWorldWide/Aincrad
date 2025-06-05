// WelcomeSequence.swift
// Aincrad Terminal
//
// Dependencies: SwiftUI, AVFoundation
// Usage: Initial view for the app
// Changelog: Initial scaffold

import SwiftUI
import AVFoundation

struct WelcomeSequence: View {
    @State private var opacity: Double = 0
    @State private var text: String = "Link Start..."

    var body: some View {
        ZStack {
            Color.black.edgesIgnoringSafeArea(.all)
            Text(text)
                .font(.largeTitle)
                .foregroundColor(.white)
                .opacity(opacity)
                .onAppear {
                    withAnimation(.easeIn(duration: 2.0)) {
                        opacity = 1
                    }
                }
        }
    }
} 