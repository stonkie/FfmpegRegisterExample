# FfmpegRegisterExample

Demo project for call to avcodec_send_packet that doesn't free ST0 register.

Just open in Visual Studio 2022 and run. the repo includes the built version of avcodec-5.9.dll.

**IMPORTANT** : The built dlls of avcodec are included for simplicity. Rebuild and replace them if you don't trust me! ;) 

# What is it?

When calling the function avcodec_send_packet, the registers are all non-empty. This is show by the TAG Word as documented here : http://www.ray.masmcode.com/tutorial/fpuchap1.htm#tword

![](https://i.imgur.com/YsHrxhz.png)


Following this, any `fld` instruction will fail. The inline assembly demonstrate this by using those registers (and the same instructions are used at the beginning of the program to confirm that it normally works).

The final output of the program is as follows.

![](https://i.imgur.com/DXTK9ni.png)

# Impacts

This affects x86 applications, even running on a 64 bits Windows OS. It does not seem to affect x64 applications. Inline assembly isn't supported for x64 in VC++, so the sample needs to be modified a bit to show this.

Calling back into avcodec (avcodec_receive_frame for example) doesn't seem to have adverse effects. It even seems to clear the registers sometimes, although I have not identified the pattern for it yet.

The biggest impact for my use case is using avcodec within a C# application. The C# compiler generates `fld` instructions for floating point to integer conversions, which result in very weird behaviors similar to this sample.