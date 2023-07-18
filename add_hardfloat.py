Import("env")
flags = [
   "-mcpu=cortex-m4",
   "-mthumb",
   "-mfloat-abi=hard",
   "-mfpu=fpv4-sp-d16"
]
cppdef = [
   "USE_FULL_ASSERT",
   "USE_FULL_LL_DRIVER"
]
env.Append(CCFLAGS=flags, LINKFLAGS=flags, CPPDEFINES=cppdef)