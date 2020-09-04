module Styles = {
  open Css;

  let topBarContainer =
    style([
      display(`flex),
      justifyContent(`center),
      alignItems(`center),
      width(`percent(100.)),
      padding2(~v=Spacing.lg, ~h=`zero),
      backgroundColor(Colors.white),
      borderBottom(`px(2), `solid, Colors.blueGray1),
      top(`zero),
      zIndex(3),
      Media.mobile([padding(Spacing.md), justifyContent(`spaceBetween), position(`fixed)]),
    ]);

  let pageWidth = style([maxWidth(`px(Config.pageWidth))]);

  let rFlex = style([display(`flex), flexDirection(`row), alignItems(`center)]);

  let cFlex = style([display(`flex), flexDirection(`column)]);

  let topBarInner =
    style([
      display(`flex),
      width(`percent(100.)),
      justifyContent(`spaceBetween),
      padding2(~v=`zero, ~h=`px(15)),
    ]);

  let logoContainer = style([display(`flex), alignItems(`center), width(`percent(100.))]);

  let bandLogo = style([width(`px(40)), Media.mobile([width(`px(34))])]);
  let twitterLogo = style([width(`px(15))]);
  let telegramLogo = style([width(`px(15))]);
  let cmcLogo = style([width(`px(17))]);

  let socialLink =
    style([
      display(`flex),
      flexDirection(`row),
      justifyContent(`center),
      alignItems(`center),
      marginLeft(`px(10)),
    ]);

  let link = style([cursor(`pointer)]);
};

module LinkToHome = {
  [@react.component]
  let make = (~children) => {
    <Link className=Styles.link route=Route.HomePage> children </Link>;
  };
};

module DesktopRender = {
  [@react.component]
  let make = () => {
    <div className=Styles.topBarContainer>
      <div className={Css.merge([Styles.topBarInner, Styles.pageWidth])}>
        <div className=Styles.logoContainer>
          <Row>
            <Col>
              <LinkToHome> <img src=Images.bandLogo className=Styles.bandLogo /> </LinkToHome>
            </Col>
            <Col> <HSpacing size=Spacing.sm /> </Col>
            <Col>
              <LinkToHome>
                <Text
                  value="BandChain"
                  size=Text.Xxl
                  weight=Text.Bold
                  nowrap=true
                  color=Colors.gray8
                  spacing={Text.Em(0.05)}
                />
              </LinkToHome>
              <VSpacing size=Spacing.xs />
              <div className=Styles.rFlex>
                <LinkToHome>
                  <Text
                    value="CosmoScan"
                    nowrap=true
                    size=Text.Sm
                    weight=Text.Semibold
                    color=Colors.gray6
                    spacing={Text.Em(0.03)}
                  />
                  <HSpacing size=Spacing.xs />
                </LinkToHome>
                <ChainIDBadge />
              </div>
            </Col>
            <Col alignSelf=Col.End>
              <div className=Styles.rFlex>
                <div className=Styles.socialLink>
                  <a href="https://twitter.com/bandprotocol" target="_blank" rel="noopener">
                    <img src=Images.twitterLogo className=Styles.twitterLogo />
                  </a>
                </div>
                <div className=Styles.socialLink>
                  <a href="https://t.me/bandprotocol" target="_blank" rel="noopener">
                    <img src=Images.telegramLogo className=Styles.telegramLogo />
                  </a>
                </div>
                <div className=Styles.socialLink>
                  <a href="https://coinmarketcap.com/currencies/band-protocol/" target="_blank" rel="noopener">
                    <img src=Images.cmcLogo className=Styles.telegramLogo />
                  </a>
                </div>
              </div>
            </Col>
          </Row>
        </div>
        <SearchBar />
      </div>
    </div>;
  };
};

module MobileRender = {
  [@react.component]
  let make = () => {
    <div className=Styles.topBarContainer>
      <Row>
        <Col>
          <LinkToHome> <img src=Images.bandLogo className=Styles.bandLogo /> </LinkToHome>
        </Col>
        <Col>
          <LinkToHome>
            <div className=Styles.cFlex>
              <Text
                value="BandChain"
                size=Text.Lg
                weight=Text.Bold
                nowrap=true
                color=Colors.gray8
                spacing={Text.Em(0.05)}
              />
              <VSpacing size=Spacing.xs />
              <Text
                value="CosmoScan"
                nowrap=true
                size=Text.Sm
                color=Colors.gray6
                spacing={Text.Em(0.03)}
              />
            </div>
          </LinkToHome>
        </Col>
      </Row>
      <Row justify=Row.Right>
        <Col> <ChainIDBadge /> </Col>
        <Col> <HSpacing size=Spacing.xs /> </Col>
        <Col> <NavBar /> </Col>
      </Row>
    </div>;
  };
};

[@react.component]
let make = () => {
  Media.isMobile() ? <MobileRender /> : <DesktopRender />;
};
