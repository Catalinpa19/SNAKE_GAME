[xml]$project = Get-Content SnakeUI/SnakeUI.vcxproj
$ns = New-Object System.Xml.XmlNamespaceManager($project.NameTable)
$ns.AddNamespace('m', 'http://schemas.microsoft.com/developer/msbuild/2003')
$project.SelectSingleNode('//m:ProjectGuid', $ns).InnerText = '{754c2e47-2e89-44ba-91a2-aa7d9f87666c}'
$project.SelectSingleNode('//m:Keyword', $ns).InnerText = 'QtVS_v304'
foreach ($node in $project.SelectNodes('//m:QtInstall', $ns)) { $node.InnerText = 'C:\Qt\6.12.0\msvc2022_64' }
foreach ($node in $project.SelectNodes('//m:QtModules', $ns)) { $node.InnerText += ';testlib' }
foreach ($node in $project.SelectNodes('//m:SubSystem', $ns)) { $node.InnerText = 'Console' }
foreach ($node in @($project.SelectNodes('//m:ItemGroup[@Label="resources"] | //m:ResourceCompile | //m:ClCompile[@Include="main.cpp"]', $ns))) { [void]$node.ParentNode.RemoveChild($node) }
foreach ($node in $project.SelectNodes('//m:ItemGroup/*[@Include]', $ns)) { $node.SetAttribute('Include', '..\SnakeUI\' + $node.GetAttribute('Include')) }
foreach ($node in $project.SelectNodes('//m:AdditionalIncludeDirectories', $ns)) {
    $node.InnerText = '$(ProjectDir)..\SnakeUI;$(ProjectDir)..\packages\gmock.1.11.0\lib\native\include;$(ProjectDir)..\packages\gmock.1.11.0\lib\native\src\gtest;$(ProjectDir)..\packages\gmock.1.11.0\lib\native\src\gmock;%(AdditionalIncludeDirectories)'
}
$group = $project.CreateElement('ItemGroup', $project.DocumentElement.NamespaceURI)
foreach ($file in @('main.cpp', 'test.cpp', 'ProfileTests.cpp', '..\packages\gmock.1.11.0\lib\native\src\gtest\gtest-all.cc', '..\packages\gmock.1.11.0\lib\native\src\gmock\gmock-all.cc')) {
    $node = $project.CreateElement('ClCompile', $project.DocumentElement.NamespaceURI)
    $node.SetAttribute('Include', $file)
    [void]$group.AppendChild($node)
}
foreach ($file in @('MockInterfaces.h', 'pch.h')) {
    $node = $project.CreateElement('ClInclude', $project.DocumentElement.NamespaceURI)
    $node.SetAttribute('Include', $file)
    [void]$group.AppendChild($node)
}
[void]$project.DocumentElement.AppendChild($group)
$project.Save((Join-Path $PWD 'SnakeTests/SnakeTests.vcxproj'))
