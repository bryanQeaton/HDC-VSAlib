#include <iostream>
#include "HDV.h"

std::vector<std::vector<std::string>> data = {
    {"life", "existence", "breath", "energy", "vitality", "being", "soul", "birth", "growth", "experience", "journey"},
    {"death", "end", "mortality", "passing", "demise", "extinction", "afterlife", "finality", "loss", "oblivion", "rest"},
    {"love", "affection", "passion", "devotion", "romance", "care", "fondness", "adoration", "yearning", "intimacy", "compassion"},
    {"hate", "anger", "rage", "resentment", "hostility", "animosity", "disgust", "loathing", "bitterness", "enmity", "aversion"},
    {"light", "brightness", "illumination", "radiance", "glow", "beam", "shine", "luminosity", "daylight", "sun", "clarity"},
    {"dark", "shadow", "night", "obscurity", "gloom", "blackness", "void", "absence", "murk", "shade", "dusk"},
    {"time", "duration", "moment", "clock", "temporal", "sequence", "era", "age", "period", "interval", "eternity", "life"},
    {"space", "universe", "cosmos", "void", "expanse", "distance", "area", "vacuum", "galaxy", "infinity", "nothingness"},
    {"water", "liquid", "flow", "river", "ocean", "rain", "moisture", "hydration", "wave", "current", "tide"},
    {"fire", "flame", "heat", "blaze", "inferno", "combustion", "spark", "ember", "ignition", "warmth", "energy"},
    {"earth", "ground", "soil", "land", "planet", "world", "terra", "dirt", "nature", "foundation", "stone"},
    {"air", "wind", "breeze", "atmosphere", "sky", "breath", "oxygen", "current", "gale", "zephyr", "space"},
    {"mind", "thought", "consciousness", "intellect", "psyche", "cognition", "brain", "reason", "perception", "awareness", "soul"},
    {"soul", "spirit", "essence", "being", "core", "inner self", "psyche", "consciousness", "life", "mind", "identity"},
    {"knowledge", "wisdom", "understanding", "information", "learning", "education", "awareness", "insight", "cognition", "mind"},
    {"power", "strength", "force", "might", "energy", "authority", "control", "dominance", "influence", "potency", "fire"},
    {"freedom", "liberty", "autonomy", "independence", "liberation", "free will", "emancipation", "self-determination", "space"},
    {"justice", "fairness", "equity", "righteousness", "law", "morality", "truth", "honor", "integrity", "virtue"},
    {"beauty", "aesthetics", "loveliness", "grace", "elegance", "attractiveness", "splendor", "radiance", "perfection", "art"},
    {"truth", "reality", "fact", "veracity", "authenticity", "honesty", "accuracy", "certainty", "knowledge", "light"},
    {"fear", "anxiety", "dread", "terror", "panic", "phobia", "apprehension", "worry", "horror", "dark"},
    {"joy", "happiness", "delight", "elation", "bliss", "euphoria", "pleasure", "contentment", "cheer", "love"},
    {"sadness", "sorrow", "grief", "melancholy", "despair", "depression", "misery", "unhappiness", "pain", "loss"},
    {"anger", "rage", "fury", "wrath", "irritation", "outrage", "hostility", "resentment", "indignation", "hate"},
    {"peace", "tranquility", "calm", "serenity", "harmony", "quiet", "stillness", "accord", "repose", "water"},
    {"chaos", "disorder", "confusion", "turmoil", "anarchy", "bedlam", "pandemonium", "mayhem", "entropy", "fire"},
    {"order", "structure", "system", "organization", "arrangement", "pattern", "sequence", "harmony", "method", "time"},
    {"change", "transformation", "alteration", "evolution", "transition", "shift", "metamorphosis", "conversion", "life"},
    {"stability", "steadiness", "constancy", "permanence", "durability", "reliability", "solidity", "foundation", "earth"},
    {"movement", "motion", "action", "activity", "flow", "progress", "travel", "journey", "dynamics", "water"},
    {"rest", "stillness", "pause", "break", "calm", "repose", "inactivity", "peace", "stop", "death"},
    {"sound", "noise", "audio", "vibration", "tone", "acoustics", "music", "echo", "resonance", "air"},
    {"silence", "quiet", "hush", "stillness", "peace", "calm", "muteness", "tranquility", "absence", "dark"},
    {"color", "hue", "shade", "tint", "pigment", "spectrum", "rainbow", "palette", "chroma", "light"},
    {"shape", "form", "figure", "contour", "structure", "outline", "geometry", "pattern", "design", "earth"},
    {"size", "dimension", "magnitude", "scale", "proportion", "extent", "volume", "measure", "space"},
    {"temperature", "heat", "cold", "warmth", "thermal", "climate", "weather", "fire", "ice", "energy"},
    {"texture", "feel", "surface", "tactile", "roughness", "smoothness", "grain", "fabric", "consistency", "earth"},
    {"speed", "velocity", "pace", "rapidity", "quickness", "swiftness", "acceleration", "movement", "time"},
    {"weight", "mass", "heaviness", "gravity", "density", "burden", "load", "substance", "earth", "stone"},
    {"memory", "recollection", "remembrance", "recall", "nostalgia", "past", "history", "mind", "time"},
    {"dream", "vision", "fantasy", "aspiration", "imagination", "illusion", "hope", "ambition", "mind", "sleep"},
    {"reality", "existence", "actuality", "truth", "fact", "world", "life", "material", "concrete", "earth"},
    {"illusion", "deception", "mirage", "fantasy", "delusion", "apparition", "dream", "trick", "light", "dark"},
    {"beginning", "start", "origin", "commencement", "genesis", "dawn", "birth", "initiation", "first", "life"},
    {"end", "conclusion", "termination", "finish", "closure", "finale", "completion", "death", "last", "time"},
    {"growth", "development", "expansion", "increase", "progress", "maturation", "evolution", "life", "change"},
    {"decay", "decline", "deterioration", "rot", "decomposition", "degeneration", "corruption", "death", "time"},
    {"connection", "link", "bond", "relationship", "tie", "attachment", "union", "love", "friendship", "life"},
    {"separation", "division", "distance", "isolation", "disconnection", "gap", "rift", "break", "death", "space"}
};

int main() {
    auto model=hdc::HDVmap();




    return 0;
}