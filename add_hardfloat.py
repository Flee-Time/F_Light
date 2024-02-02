Import("env")
flags = [
   "-mcpu=cortex-m4",
   "-mthumb",
   "-mfloat-abi=hard",
   "-mfpu=fpv4-sp-d16"
]
env.Append(CCFLAGS=flags, LINKFLAGS=flags)