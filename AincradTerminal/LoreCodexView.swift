// LoreCodexView.swift
// Aincrad Terminal
//
// Dependencies: SwiftUI, AVFoundation
// Usage: Displays lore entries with voice playback
// Changelog: Initial scaffold

import SwiftUI
import AVFoundation

struct LoreEntry: Codable {
    let title: String
    let content: String
}

struct LoreCodexView: View {
    @State private var loreEntries: [LoreEntry] = []
    @State private var selectedEntry: LoreEntry?
    @State private var isExpanded: Bool = false
    private let synthesizer = AVSpeechSynthesizer()

    var body: some View {
        ScrollView {
            VStack {
                ForEach(loreEntries, id: \.title) { entry in
                    VStack {
                        Text(entry.title)
                            .font(.headline)
                            .onTapGesture {
                                selectedEntry = entry
                                isExpanded.toggle()
                                speak(entry.content)
                            }
                        if isExpanded && selectedEntry?.title == entry.title {
                            Text(entry.content)
                                .font(.body)
                        }
                    }
                    .padding()
                }
            }
        }
        .onAppear {
            loadLoreEntries()
        }
    }

    private func loadLoreEntries() {
        // Load sample lore entries from local JSON
        // Placeholder data
        loreEntries = [
            LoreEntry(title: "The Beginning", content: "In the world of Aincrad, players are trapped in a virtual reality game."),
            LoreEntry(title: "The Black Swordsman", content: "A legendary player known for his unmatched skills.")
        ]
    }

    private func speak(_ text: String) {
        let utterance = AVSpeechUtterance(string: text)
        synthesizer.speak(utterance)
    }
} 