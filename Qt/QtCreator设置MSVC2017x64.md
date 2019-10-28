* 打开`工具-选项-构建和运行-构建套件(Kit)`，选中MSVC2017，感叹号表示未配置好，没设置的原因是缺少Windows的调试器，这需要下载[Win10SDK下载器](https://developer.microsoft.com/en-US/windows/downloads/windows-10-sdk)
* 打开安装程序，只选择所需的`Debugging Tools for Windows`即可，然后点击`Download`将下载安装包
* 下载完成后，在下载目录中可以看到一个`Windows Kits`文件夹（如果没有则可能直接完成安装，跳到下下步打开QtCreator检测）
* 打开`...\Windows Kits\10\WindowsSDK\Installers\X64 Debuggers And Tools-x64_en-us`，即可安装
* 安装完成后，QtCreator中将自动检测到，打开`工具-选项-构建和运行-Debuggers`，可以看到自动检测中多了一个CDB调试器。若未自动检测到也可以点击右侧的`Add`手动添加，路径是`C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\cdb.exe`
* 再点击`构建套件(Kit)`，选中MSVC2017，调试器选择CDB即可
* 另外注意编译器也要选择64位（amd64），不然出现的是红色感叹号（amd64_x86表示cross-complier，即交叉编译，用于64位平台开发32位程序）
