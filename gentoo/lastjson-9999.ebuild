EAPI=4
inherit git-2

EGIT_REPO_URI="git://github.com/lastfm/last.json.git"

DESCRIPTION="Last.fm's C++ JSON library"
HOMEPAGE="https://github.com/lastfm/last.json"
SRC_URI=""
SLOT="live"

LICENSE="MIT"
KEYWORDS="~x86 ~amd64"

DEPEND="dev-libs/boost"

src_install() {
	insinto /usr/include
	doins -r include/lastjson
}
