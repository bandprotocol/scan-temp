module Styles = {
  open Css;

  let bg =
    style([
      width(`percent(100.)),
      height(`px(200)),
      left(`zero),
      bottom(`zero),
      position(`relative),
      background(hex("F6F3FA")),
      before([
        position(`absolute),
        contentRule(`none),
        background(hex("F6F3FA")),
        width(`percent(100.)),
        height(`px(300)),
        transform(`skewY(`deg(6.))),
        zIndex(-1),
        top(`px(-200)),
      ]),
    ]);

  let footerContainer =
    style([
      marginTop(Spacing.xl),
      maxWidth(`px(984)),
      marginLeft(`auto),
      marginRight(`auto),
      paddingLeft(Spacing.md),
      paddingRight(Spacing.md),
    ]);

  let vFlex = style([display(`flex), flexDirection(`column)]);

  let logo = style([width(`px(35))]);

  let logoContainer = style([width(`px(50))]);

  let messageBlock = style([maxWidth(`px(220)), lineHeight(`px(20))]);

  let subHeader = style([height(`px(40)), display(`flex), alignItems(`center)]);

  let commonLink = style([marginBottom(Spacing.md), textDecoration(`none), display(`block)]);
};

let footerData = [|
  (
    "D3N Project",
    1.0, // flex size
    [|
      ("https://bandprotocol.com", "Band Protocol Website"),
      // ("https://scan.d3n.bandprotocol.com", "D3N Blockchain"),
      // ("https://metamask.io", "D3N Wallet"),
    |],
  ),
  (
    "COMMUNITY",
    1.0,
    [|
      ("https://github.com/bandprotocol/d3n", "Open Source Repository"),
      ("https://github.com/bandprotocol/d3n/tree/master/spec", "Developer Docs"),
      // ("https://etherscan.com", "Network Status"),
    |],
  ),
  (
    "SOCIAL LINKS",
    0.5,
    [|
      ("https://twitter.com/bandprotocol", "Twitter"),
      ("https://t.me/bandprotocol", "Telegram"),
      ("https://medium.com/bandprotocol", "Medium"),
    |],
  ),
|];

let renderSubHeader = text =>
  <>
    <div className=Styles.subHeader>
      <Text value=text size=Text.Md weight=Text.Bold color=Colors.gray5 />
    </div>
    <VSpacing size=Spacing.lg />
  </>;

let renderCommonLink = (url, text) =>
  <a href=url target="_blank" rel="noopener" key=url className=Styles.commonLink>
    <Text value=text size=Text.Md weight=Text.Semibold color=Colors.purple7 />
  </a>;

[@react.component]
let make = () => {
  <div className=Styles.bg>
    <div className=Styles.footerContainer>
      <Row justify=Row.Between>
        <Col size=1.5>
          <div className=Styles.subHeader>
            <Row>
              <div className=Styles.logoContainer>
                <Col size=1.> <img src=Images.bandLogo className=Styles.logo /> </Col>
              </div>
              <Col size=4.>
                <div className=Styles.vFlex>
                  <Text value="POWERED BY" size=Text.Sm weight=Text.Semibold nowrap=true />
                  <Text value="Band Protocol" size=Text.Xxxl weight=Text.Bold nowrap=true />
                </div>
              </Col>
            </Row>
          </div>
          <VSpacing size=Spacing.lg />
          <div className=Styles.messageBlock>
            <Text
              value="D3N is a block explorer and an analytic platform for Band Protocol, a decentralized platform for reliable and provably-secure data on blockchain."
              size=Text.Md
              weight=Text.Regular
              block=true
            />
          </div>
        </Col>
        {footerData->Belt.Array.map(((header, size, links)) =>
           <Col key=header size>
             {renderSubHeader(header)}
             {links->Belt.Array.map(((url, text)) => renderCommonLink(url, text)) |> React.array}
           </Col>
         )
         |> React.array}
      </Row>
    </div>
  </div>;
};
