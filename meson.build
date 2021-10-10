project(
	'lua-aml',
	'c',
	version: '0.1.0',
	license: 'ISC',
	default_options: [
		'c_std=c11',
	]
)

buildtype = get_option('buildtype')

c_args = [
	'-DPROJECT_VERSION="@0@"'.format(meson.project_version()),
	'-D_POSIX_C_SOURCE=200809L',
	'-fvisibility=hidden',
]

git = find_program('git', native: true, required: false)
if git.found()
	git_describe = run_command([git, 'describe', '--tags', '--long'])
	git_branch = run_command([git, 'rev-parse', '--abbrev-ref', 'HEAD'])
	if git_describe.returncode() == 0 and git_branch.returncode() == 0
		c_args += '-DGIT_VERSION="@0@ (@1@)"'.format(
			git_describe.stdout().strip(),
			git_branch.stdout().strip(),
		)
	endif
endif

if buildtype != 'debug' and buildtype != 'debugoptimized'
	c_args += '-DNDEBUG'
endif

add_project_arguments(c_args, language: 'c')

sources = [
	'src/lua-aml.c',
]

lua = dependency('luajit')
aml = dependency('aml')

dependencies = [
	lua,
	aml,
]

lua_aml = shared_module(
	'aml',
	sources,
	dependencies: dependencies,
	name_prefix: '',
	install: true,
)
