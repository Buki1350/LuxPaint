#include "Initializable.h"

#include "Initializables.h"

Initializable::Initializable() { Initializables::AddInitializable(this); }