// CharacterCreationView.swift
// Aincrad Terminal
//
// Dependencies: SwiftUI, CloudKit
// Usage: Character creation and profile saving
// Changelog: Initial scaffold

import SwiftUI
import CloudKit

struct CharacterCreationView: View {
    @State private var name: String = ""
    @State private var swordStyle: String = ""
    @State private var personalityTrait: String = ""

    var body: some View {
        VStack {
            TextField("Name", text: $name)
                .textFieldStyle(RoundedBorderTextFieldStyle())
            TextField("Sword Style", text: $swordStyle)
                .textFieldStyle(RoundedBorderTextFieldStyle())
            TextField("Personality Trait", text: $personalityTrait)
                .textFieldStyle(RoundedBorderTextFieldStyle())
            Button("Save Profile") {
                saveProfile()
            }
        }
        .padding()
    }

    private func saveProfile() {
        let record = CKRecord(recordType: "CharacterProfile")
        record["name"] = name
        record["swordStyle"] = swordStyle
        record["personalityTrait"] = personalityTrait

        let container = CKContainer.default()
        let database = container.privateCloudDatabase

        database.save(record) { (record, error) in
            if let error = error {
                print("Error saving profile: \(error.localizedDescription)")
            } else {
                print("Profile saved successfully")
            }
        }
    }
} 