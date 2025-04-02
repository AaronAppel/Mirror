TODO list for features, cleanup, documentation, etc.

### MIRROR_TYPE_SIZE_UNUSED && MIRROR_FIELD_FLAGS_UNUSED
The macros MIRROR_TYPE_SIZE_UNUSED and MIRROR_FIELD_FLAGS_UNUSED are declared at the top of Structs.h, and wrap the 2 fields in the Mirror struct, but don't see support elsewhere in the Mirror project or example test App project like de/serialization files.
1. \#Define the macros and see what breaks, and/or find all references to the 2 fields
2. Wrap all relevant code sections with the macros
3. Fix any additional logic errors or considerations
4. Add/update documentation as needed

## Primitive Type ID values
Could look at giving users values starting at 0, and use MIRROR_FIELD_ID_SIZE_MAX - 0, -1, etc instead for pre-defined primitive type IDs.
### Debugging Logic
Error handling/checking and user error message output should be consistent. Go through all asserts, log statements, and error handling logic to ensure consistency and validity.
Review MIRROR_ASSERT for usage.

### Example App Demos and Testing
Review the App project for user examples and test cases. Examples should be applicable and concise. Test cases should support catching simple mistakes while developing Mirror.

### #TODO Comments in code
Consider removing most comments in the source and replacing them with references to documentation. Maybe a short blurb in code will suffice, but expanded reasoning and examples can be given in documentation, while removing lengthy code comments.

# Get type from ID
#TODO Possible feature? Would it be useful to get a TypeInfo* for a type given only its ID? It might be a pain to implement, and very situational, but also cool to have if needed.

# Serialization example
#TODO Serialization was removed for simplicity, at least on next submit of major changes. Look at bringing back a simple serialization example in the future for users to reference.

# VS project filter files
#TODO Setup VS folder or file view in build settings so new repos have a good file structure. Ignore .filters files so users aren't forced to use, delete, or override mine. They shouldn't be important enough to commit to Git.