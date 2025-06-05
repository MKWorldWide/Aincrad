// FloorBossCountdown.swift
// Aincrad Terminal
//
// Dependencies: SwiftUI
// Usage: Countdown to boss event
// Changelog: Initial scaffold

import SwiftUI

struct FloorBossCountdown: View {
    @State private var timeRemaining: TimeInterval = 3600 // 1 hour in seconds
    @State private var timer: Timer?
    @State private var showMessage: Bool = false

    var body: some View {
        VStack {
            Text("Time Remaining: \(formatTime(timeRemaining))")
                .font(.title)
                .padding()
            if showMessage {
                Text("The Black Swordsman has entered the floor...")
                    .font(.headline)
                    .foregroundColor(.red)
            }
        }
        .onAppear {
            startTimer()
        }
    }

    private func startTimer() {
        timer = Timer.scheduledTimer(withTimeInterval: 1, repeats: true) { _ in
            if timeRemaining > 0 {
                timeRemaining -= 1
            } else {
                showMessage = true
                timer?.invalidate()
            }
        }
    }

    private func formatTime(_ timeInterval: TimeInterval) -> String {
        let hours = Int(timeInterval) / 3600
        let minutes = Int(timeInterval) / 60 % 60
        let seconds = Int(timeInterval) % 60
        return String(format: "%02d:%02d:%02d", hours, minutes, seconds)
    }
} 