# Copyright 2018 Bateared Collie
#
# Redistribution and use in source and binary forms, with or without modification, 
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this 
#   list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice, this 
#   list of conditions and the following disclaimer in the documentation and/or other
#   materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors may 
#   be used to endorse or promote products derived from this software without specific
#   prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
# SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
# OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
# TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
'''
Set up module 
'''

#from distutils.command.build_py import build_py as _build_py
from distutils.core import setup
from setuptools.command.install import install
from setuptools.command.build_py import build_py

class CustomBuildCommand(build_py):
    user_options = install.user_options + [
         ('without-madagascar-extension=', None, 'Disables the installation of Madagascar extensions, use =True to enable')
     ]
    #boolean_options = install.boolean_options +["without-madagascar-extension"]
 
    def initialize_options(self):
        build_py.initialize_options(self)
        self.without_madagascar_extension = None
 
    def run(self):
        global without_madagascar_extension
        without_madagascar_extension = self.without_madagascar_extension
        if without_madagascar_extension!=None:
            print "\nDisabling Madagascar extension at users request"
            self.__dict__["distribution"].__dict__["packages"].remove('stk.mdExt')
            self.__dict__["distribution"].__dict__["exclude"].append('stk.mdExt')
        build_py.run(self)  # OR: install.do_egg_install(self)

class CustomInstallCommand(install):
    user_options = install.user_options + [
         ('without-madagascar-extension=', None, 'Disables the installation of Madagascar extensions, use =True to enable')
     ]
    #boolean_options = install.boolean_options +["without-madagascar-extension"]

    def initialize_options(self):
        install.initialize_options(self)
        self.without_madagascar_extension = None

    def finalize_options(self):
        #print('The custom option for install is ', self.custom_option)
        install.finalize_options(self)

    def run(self):
        global without_madagascar_extension
        without_madagascar_extension = self.without_madagascar_extension
        if without_madagascar_extension!=None:
            print "\nDisabling Madagascar extension at users request"
            self.__dict__["distribution"].__dict__["packages"].remove('stk.mdExt')
            self.__dict__["distribution"].__dict__["exclude"].append('stk.mdExt')
        install.run(self)  # OR: install.do_egg_install(self)

setup(
    name ='stk',
    version ='1.0',
    description= 'SeisTK Python bindings',
    author ='Bateared Collie',
    author_email = 'BatearedCollie@gmail.com',
    packages=['stk','stk.generators','stk.mdExt','stk.obspyExt'],
    exclude=[],
    cmdclass={
        'install': CustomInstallCommand,
        'build_py': CustomBuildCommand,
        }
    
    )
